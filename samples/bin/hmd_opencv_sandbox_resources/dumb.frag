#version 430 core

layout(location = 8) uniform sampler2D mytexture;
in vec4 colorTrans;
out vec4 outputColor;
void main()
{
   outputColor = vec4( colorTrans.rgba  );
}