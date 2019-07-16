#version 330 core
out vec4 FragColor; 

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 lightColour;

void main() {
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * lightColour;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;
    
	vec4 lighting = vec4(ambient + diffuse, 1.0);
	FragColor = lighting * texture(texture_diffuse1, TexCoord);


}
