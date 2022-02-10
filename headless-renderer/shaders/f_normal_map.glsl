// Fragment shader: : Normal mapping shading
// ================
#version 450 core

in vec2 _TexCoord;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

// Ouput data
out vec4 FragColor;

//uniforms
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct PointLight{
	vec3 position;
	vec3 color;
	float power;
 };

uniform Material material;
uniform PointLight light;

#define linear_coef 0.01
#define quadratic_coef 0.01

void main()
{    
        // ambient
	vec3 ambient = material.ambient;    
    //read from normalMap the normal vector
    vec3 normal = texture(normalMap, _TexCoord).rgb;
    //remap [0,1]->[-1,1]
    normal = normal * 2 -1; // 2=(1-(-1))
    vec3 color = texture(diffuseMap, _TexCoord).rgb;

    // diffuse 
	vec3 lightDir = normalize(TangentLightPos-TangentFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.power * light.color * diff * color;
    
    // specular
    vec3 viewDir = normalize(TangentViewPos-TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  light.power * light.color * spec * material.specular;  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}