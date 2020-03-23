#version 430

layout(location = 0) uniform mat4 matrix;
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 v2UVcoordsIn;

out vec2 v2UVcoords;
out vec3 opos;
void main()
{
	v2UVcoords = v2UVcoordsIn*2.0-1.0;
	opos = position;
	//gl_Position = matrix * vec4( position, 1.0 ) * ( vec4( 1., 1., 0., 1. ) /* Tricky: Don't far clip. */ );
	gl_Position = matrix * vec4( position, 1.0 );
}