#version 450 core

layout(location = 0) out vec4 outColor;

layout(location = 0) smooth in vec3 color;

void main()
{
    outColor = vec4(0.196, 0.6588, 0.2588, 1.0);
}