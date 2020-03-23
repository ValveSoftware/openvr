#version 430 core
uniform sampler2D mytexture;
in vec2 v2UVcoords;
in vec3 opos;
out vec4 outputColor;
layout(location = 7) uniform vec4 coloruniform;
void main()
{
   vec2 grid = clamp(  sin( v2UVcoords*6000.0 )*60.0 - 58.0, vec2(0.0), vec2(1.0) ); //Make grid doodad
	//vec2 dgrid = sqrt( abs( dFdy( v2UVcoords ) +  dFdx( v2UVcoords ) ) ); //Fade out colors to avoid aliasing by using derivitive.
   float dgrid = 4.0 - length( opos.xz )*.5;
   outputColor = vec4( ( vec3( length(grid) ) *0.1 + 0.05 ) * clamp (dgrid,0.0,1.0), 1.0 ) * coloruniform;
   
   
}
