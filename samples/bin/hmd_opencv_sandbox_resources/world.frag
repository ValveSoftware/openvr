#version 430 core

uniform sampler2D mytexture;
in vec2 v2UVcoords;
in vec4 posout;
uniform vec3 tolforconf;
layout(location = 7) uniform vec4 coloruniform;
out vec4 outputColor;

void main()
{
	float forcefloorconf = 1.0;
	if( posout.y < tolforconf.z ) forcefloorconf = (posout.y - tolforconf.z)*10.0+ 1.0;
	outputColor = vec4( pow( texture(mytexture, v2UVcoords).rgb * vec3(1.0,1.0,1.0), vec3(1.0) ), 
		clamp(posout.w * tolforconf.x + tolforconf.y, 0.0, 1.0)*forcefloorconf ) * coloruniform;
}
