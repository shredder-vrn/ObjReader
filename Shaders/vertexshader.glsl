#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 openGLcurrentMvp;

out vec2 outTexCoord;
out vec3 fragNormal;

void main()
{
    fragNormal = normal;
    outTexCoord = inTexCoord;
    gl_Position = openGLcurrentMvp * vec4(position, 1.0);
}
