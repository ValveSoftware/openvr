// vertex shader for terminal
#version 430
layout(location = 0) uniform mat4 matrix;
layout(location = 1) uniform mat4 modelview;
layout(location = 21) uniform mat4 modelviewPre;
layout(location = 20) uniform vec4 tprops;

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 v2TexCoordsIn;
out vec2 v2TexCoord;
void main()
{
	v2TexCoord = v2TexCoordsIn;
	vec4 newbase = modelviewPre * vec4(position.xyz, 1);
	vec4 newpos = modelview * vec4(newbase.xyz, 1);
	gl_Position = matrix * newpos;
}