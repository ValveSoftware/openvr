#version 430

layout(location = 0) uniform mat4 matrix;

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 v2UVcoordsIn;

out vec2 v2UVcoords;
out vec4 posout;
void main()
{
	vec4 lpos = position;
	posout = lpos;
	v2UVcoords = v2UVcoordsIn;
	gl_Position = matrix * vec4( lpos.xyz, 1.0 );
}
