//fragment shader for terminal
#version 430 core
layout(location = 8) uniform sampler2D font;
layout(location = 9) uniform sampler2D text;
layout(location = 20) uniform vec4 tprops;

in vec2 v2TexCoord;
out vec4 outputColor;
void main()
{
	vec2 rightcoord = vec2( v2TexCoord.x, 1.0 - v2TexCoord.y );
	vec2 rightcoordlookup = ( floor( rightcoord * tprops.zw ) + 0.5  ) / tprops.zw;
	vec4 chardata = texture( text, rightcoordlookup );
	vec2 subchar = mod( rightcoord * tprops.zw, 1.0 );

	vec2 placeinsubtex = (subchar * 16.0 / tprops.yy);	//Find the coarse position in the submap.
	
	//Add in thee coarse position.
	placeinsubtex += vec2( mod( chardata.x * 255.0, 16.0 )/16.0, floor( chardata.x * 255.0 / 16.0 )/16.0 );
	
	//Make it look like no filtering (sharp edges)
	//placeinsubtex = floor( placeinsubtex * tprops.xy ) / tprops.xy + 0.5/tprops.xy;
	
	vec2 ofs = mod( placeinsubtex * tprops.xy, 1.0 ) / tprops.xy;
	placeinsubtex -= pow( ofs, vec2( 1.2 ) );// + 0.5/tprops.xy;
	
	//placeinsubtex = floor( placeinsubtex * tprops.xy ) / tprops.xy  + 0.5/tprops.xy;
	
	vec4 textdata = texture( font, placeinsubtex );
	
	textdata.x = pow( textdata.x * 2.0, 2.0 );
	textdata.x = clamp( textdata.x, 0.0, 1.0 );
	
	float charcolorinfo = chardata.b * 255.0 + 0.5;
	vec3 bg = vec3( 
		floor( mod( charcolorinfo, 32.0 ) / 16.0 ),
		floor( mod( charcolorinfo, 64.0 ) / 32.0 ),
		floor( mod( charcolorinfo, 128.0 ) /  64.0 ) );
	vec3 fg = vec3(
		floor( mod( charcolorinfo, 2.0 ) / 1.0 ),
		floor( mod( charcolorinfo, 4.0 ) / 2.0 ),
		floor( mod( charcolorinfo, 8.0 ) /  4.0 ) );
	float charprops = chardata.g * 255.0 + 0.5;
	vec3 props = vec3(
		floor( mod( charprops, 2.0 ) / 1.0 ),
		floor( mod( charprops, 4.0 ) / 2.0 ),
		floor( mod( charprops, 32.0 ) / 16.0 ) );
	if( props.z > 0.5 )
	{
		vec3 tmp = bg;
		bg = fg;
		fg = tmp;
	}
	
	if( props.x < 0.5 )
	{
		//Not bright
		fg *= .8;
	}
	if( props.y > 0.5 )
	{
		fg *= .3;
	}
	outputColor = vec4( mix( bg, fg, textdata.x ), 1.0 );
	if( textdata.x < 0.5 && length(bg) < .1 ) outputColor.a = 0.9;

#if 0
	vec2 rightcoord = vec2( v2TexCoord.x, 1.0 - v2TexCoord.y );
	//vec4 chardata =  texture( text, v2TexCoord ).xyzw;
	//outputColor = vec4( chardata, 1. );
	
	
	vec4 tvA  = texture( text, rightcoord ).xyzw;
	//vec4 tvB  = texture2D( texture0, fvpos + vec2( 0, vvExtra.w ) ); //Doing the second texture look up "looks" slower but is in fact way faster, up from 25 FPS to 42.

	vec2 targetc = tvA.xy;
	vec2 placeincharacter = mod( vv1Col.zw, 1.0 ) / 16.0;
	targetc = targetc + placeincharacter - vec2( 0.005); //This tweaks the position in the texture we're looking.

	float finalchartex = (texture2D( font, targetc )).r; //Look up texture and color-stretch.

	return;
	
/*  //For filtering
	vec3 fgcolor = vec3( tvB.yzw );
	vec3 bgcolor = vec3( tvA.zw, tvB.x );
	vec3 fg = finalchartex * fgcolor;
	vec3 bg = (1.-finalchartex) * bgcolor; ///??? Why is this faster than "mix"??
	outputColor = vec4( fg+bg, 1.0 );*/

	if( finalchartex > 0.5 )
		outputColor = vec4( tvB.yzw, 1.0 );
	else
		outputColor = vec4( tvA.zw, tvB.x, 1.0 );
		#endif
}
