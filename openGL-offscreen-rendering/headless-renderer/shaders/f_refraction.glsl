// Fragment shader: Refraction shading
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
    float ratio = 1.00 / 1.52;
    vec3 R = refract(E, normalize(Normal), ratio);
    FragColor = texture(cubemap, R);
}