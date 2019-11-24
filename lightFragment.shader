#version 330 core

out vec4 FragColor;
//in vec3 Normal;
//in vec4 gl_position;
in vec3 var_color;
//in vec4 FragPos;

//uniform vec4 u_Color;


void main()
{
	float ambientStrength = 0.4;
	vec3 ambient = ambientStrength * vec3(1f,1f,1f);
	
	/*vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(gl_position - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1.0f, 1.0f, 1.0f);*/

	vec3 result = (ambient) * var_color;
	FragColor = vec4(result, 1.0f);

};