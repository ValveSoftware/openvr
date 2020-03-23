#version 430
layout(location = 0) uniform mat4 matrix;
layout(location = 7) uniform vec4 coloruniform;
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
out vec4 colorTrans;

void main()
{
	colorTrans = color * coloruniform * vec4( 1.0, 1.0, 1.0, 1. );
	gl_Position = matrix * vec4( position, 1.0 );
}
