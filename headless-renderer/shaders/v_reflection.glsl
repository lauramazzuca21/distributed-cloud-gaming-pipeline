// Vertex shader: : Reflection shading
// ================
#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
out vec3 Normal;
out vec3 Position;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
    Normal = mat3(transpose(inverse(M))) * aNormal;
    Position = vec3(M * vec4(aPos, 1.0)); //in WCS
    gl_Position = P * V * M * vec4(aPos,1.0);
}