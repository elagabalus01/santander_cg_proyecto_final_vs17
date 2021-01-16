#version 330 core
struct DirLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform DirLight light;

in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

void main()
{
	//Text color
	vec4   texColor=texture(texture_diffuse1, TexCoords);
    if(texColor.a < 0.1)
        discard;
	vec3 texture_color=vec3(texColor.r,texColor.g,texColor.b);
	
	//Properties
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	// Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

	//Specular shadding 
	vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 15.0);

	//Ambient light
	vec3 ambient = light.ambient * texture_color;

    //Diffuselight
    vec3 diffuse = light.diffuse * diff * texture_color;
    
	//SpecularLight
    vec3 specular = light.specular * pow(spec,1) * texture_color;
	
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
	vec3 lightPower = vec3(1.0f,1.0f,1.0f);
	vec3 result =  ambient + diffuse + specular;
	//FragColor = vec4(result*intensity,1);
	FragColor = vec4(result*intensity,1);
}