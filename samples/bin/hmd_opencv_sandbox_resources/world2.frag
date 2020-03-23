#version 430 core

uniform sampler2D mytexture;
in vec2 v2UVcoords;
in vec4 posout;
in vec4 pos_xformed;
uniform vec3 tolforconf;
layout(location = 7) uniform vec4 coloruniform;
out vec4 outputColor;

#define KOOMER 0

vec3 utahcolor( float u, vec3 col, float sat )
{
	const vec3 utahC = vec3( 73.0/255.0, 177.0/255.0, 159.0/255.0 );
	const vec3 utahB = vec3( 97.0/255.0, 96.0/255.0, 155.0/255.0 );
	const vec3 utahP = vec3( 178.0/255.0, 87.0/255.0, 148.0/255.0 );
	
#if KOOMER
	u = mod( u, 1.0 );
	if( u < 0.333 )
		return mix( utahP, utahC, u * 3.0 );
	if( u < 0.666 )
		return mix( utahC, utahB, (u-.333) * 3.0 );
	return mix( utahB, utahP, (u-.666) * 3.0 );
#else
	return mix( vec3(u)/8.0, col, sat );
#endif
}

void main()
{
	vec3 decolor = pow( texture(mytexture, v2UVcoords).rgb * vec3(1.0,1.0,1.0), vec3(1.0) );
	float thinness = 50.0;

#define MODE 1
#if MODE == 0
	//Crazy Grid Mode
	float fadeout = clamp( 1.0 - length( posout.xzy )*.1, 0.0, 1.0 );
	
	
	vec3 gridalign = sin ( mod( vec3( posout.yyy ) * 20.0, 1.0 ) * 3.14159 )* thinness - (thinness-1.); 
	if( posout.y < 0.1 ) discard;
	gridalign = clamp( gridalign, 0.0, 1.0 );
	outputColor = vec4( 
		decolor,
		//W
		clamp(posout.w * tolforconf.x + tolforconf.y, 0.0, 1.0)
		* length( gridalign ) * fadeout
		) * coloruniform;

#elif MODE == 1
	//Crazy Grid Mode
	
	vec3 odecolor = decolor;
	float decoloramp = length( odecolor );
	decoloramp = clamp( decoloramp, 0.1, 1.0 );
	float derv = abs( dFdx( decoloramp ) ) + abs( dFdy( decoloramp ) );
	decolor = mix(  
		( vec3( derv.xxx * 3.0 ) * clamp(2.-pos_xformed.z*.4, 0.0, 1.0 ) +  utahcolor( posout.w * .5 + decoloramp*2.0, odecolor, 0. ) )  * vec3( 0.1, 0.6, 0.6 ),
			odecolor,  1.0 * clamp( 1.4 - pos_xformed.z, 0., 1. ) ) ;
	decolor = clamp( decolor, 0.0, 1.0 );
	vec3 gridalign = 
		clamp( 
//		sin ( mod( vec3( posout.xyz ) * 20.0, 1.0 ) * 3.14159 ) * thinness - (thinness-10.)
		vec3( 1. )
		,0., 1. );
	
	float fadeout = 1.0;//clamp( 1.5 - length( posout.xzy )*.2, 0.0, 1.0 );;
	outputColor = vec4( 
		decolor,
		//W
		clamp(  posout.w * 2.0, 0.0, 1.0)
		 * clamp( fadeout, 0.0, 1.0 ) 
		 * length( gridalign )
		) * coloruniform;
		
#endif
}

