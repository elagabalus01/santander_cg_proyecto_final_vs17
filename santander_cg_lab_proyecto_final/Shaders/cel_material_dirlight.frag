#version 330 core
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight light;

void main()
{
	// Properties
	vec3 lightDir = normalize(-light.direction);
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

	// Specular shadding
    vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0f);
    
	// Ambient
    vec3 ambient = light.ambient * material.ambient;
	
	//Diffuse light
    vec3 diffuse = light.diffuse * diff * material.diffuse;
	
	//Specular light
	vec3 specular = light.specular * spec * material.specular;
	
	//Implementando el cel shader
    float intensity = 0.6*diff+0.4*spec;
    if (intensity > 0.9) {
        intensity = 1.2;
    }
	else if (intensity > 0.8) {
        intensity = 0.9;
    }
    else if (intensity > 0.5) {
        intensity = 0.7;
    }
	/*else if (intensity > 0.3) {
        intensity = 0.5;
    }*/
    /*else if (intensity > 0.2) {
        intensity = 0.3;
    }*/
	else {
        intensity = 0.5;
    }
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f)*intensity;
}