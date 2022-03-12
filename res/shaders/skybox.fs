#version 450 core

layout(location = 0) out vec4 outColor;

layout(location = 0) smooth in vec3 texCoords;

uniform samplerCube skybox;

void main()
{
    outColor = texture(skybox, texCoords);
}