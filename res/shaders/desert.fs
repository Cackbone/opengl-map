#version 450 core

layout(location = 0) out vec4 outColor;

layout(location = 0) smooth in vec3 color;
layout(location = 1) smooth in vec2 inTexCoords;

uniform sampler2D ourTexture;

void main()
{
    vec4 texColor = texture(ourTexture, inTexCoords);
    outColor = texColor;
}