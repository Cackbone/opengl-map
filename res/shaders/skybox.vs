#version 450 core

layout(location = 0) in vec3 inPos;

layout(location = 0) out vec3 texCoords;

layout(std140, binding = 0) uniform Matrix
{
    mat4 modelViewProjection;
    vec3 lightPos;
};

void main()
{
    texCoords = inPos;
    gl_Position = modelViewProjection * vec4(inPos, 1.);
}
