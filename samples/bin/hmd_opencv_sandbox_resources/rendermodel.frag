//fragment shader for controllers
#version 430 core
layout(location = 8) uniform sampler2D diffuse;
in vec2 v2TexCoord;
out vec4 outputColor;
void main()
{
   outputColor = vec4( texture2D( diffuse, v2TexCoord).xyz, 1. );
}
