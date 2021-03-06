#version 450 core

layout(location = 0) out vec4 outColor;

layout(location = 0) smooth in vec3 color;
layout(location = 1) smooth in vec2 inTexCoords;
layout(location = 2) smooth in vec3 inNormal;
layout(location = 3) smooth in vec3 inPos;

uniform sampler2D ourTexture;
layout(std140, binding = 0) uniform Matrix
{
    mat4 modelViewProjection;
    vec3 lightPos;
};

void main()
{
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
    vec3 norm = normalize(inNormal);
    vec3 lightDir = normalize(lightPos - inPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    outColor = vec4(color * (diffuse + ambient), 1.0);
}