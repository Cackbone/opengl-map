#version 450 core

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inColor;

layout(location = 0) smooth out vec3 color;
layout(std140, binding = 0) uniform Matrix
{
    mat4 modelViewProjection;
    vec3 lightPos;
};

void main()
{
    vec3 pos = {inWorldPos.x + lightPos.x, inWorldPos.y + lightPos.y, inWorldPos.z + lightPos.z};
    color = inColor;
    gl_Position = modelViewProjection*vec4(pos, 1.);
}
