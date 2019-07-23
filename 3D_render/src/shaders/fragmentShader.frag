#version 330 core
out vec4 FragColor; 

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

struct Light {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 100

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 lightColour;
//uniform Light light;
uniform int pointLightSlotsFilled;
uniform Light pointLights[NR_POINT_LIGHTS];

uniform vec2 TexCoordShift;


vec3 CalcPointLight(Light light, vec3 normal, vec3 FragPos)
{
	vec3 ambient = light.ambient;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse;

	// attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    //ambient  *= attenuation; 
    diffuse   *= attenuation;
    return ambient + diffuse;
}

void main() {
	vec3 lighting = vec3(0.0,0.0,0.0);

	for(int i = 0; i < pointLightSlotsFilled; i++)
		lighting += CalcPointLight(pointLights[i], Normal, FragPos);

	vec2 FragTexCoord = TexCoord + TexCoordShift;

	vec4 texColour = texture(texture_diffuse1, FragTexCoord);
	if (texColour.a < 0.1)
		discard;

	

	FragColor = vec4(lighting, 1.0) * texColour;


}
