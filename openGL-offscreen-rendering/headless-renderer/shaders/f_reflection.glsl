// Fragment shader: Reflection shading
// ================
#version 450 core

out vec4 FragColor;
in vec3 Normal;
in vec3 Position;
uniform vec3 cameraPos;
uniform samplerCube cubemap;

void main()
{
    vec3 E = normalize(Position - cameraPos);
    vec3 R = reflect(E, normalize(Normal));
    FragColor = texture(cubemap, R);
}