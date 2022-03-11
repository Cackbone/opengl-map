#version 450 core

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoords;
layout(location = 3) in vec3 inNormal;

layout(location = 0) smooth out vec3 color;
layout(location = 1) smooth out vec2 texCoords;
layout(location = 2) smooth out vec3 normal;
layout(location = 3) smooth out vec3 pos;

layout(std140, binding = 0) uniform Matrix
{
    mat4 modelViewProjection;
    vec3 lightPos;
};

void main()
{
    color = inColor;
    texCoords = inTexCoords;
    normal = inNormal;
    pos = vec3(mat4(1.0) * vec4(inWorldPos, 1.));
    gl_Position = modelViewProjection*vec4(inWorldPos, 1.);
}
