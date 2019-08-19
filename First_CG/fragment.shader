#version 330 core 

out vec4 FragColor;
in vec3 FragPos;
in vec3 var_color;
in vec3 CubeNormal;
//in vec3 gl_position
//uniform vec4 u_Color;
//vec3 lightPos = v(6.5, 0.5, -0.5);

void main()
{
	float ambientStrength = 0.001;
	vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 0.0f);

	vec3 norm = normalize(CubeNormal);
	vec3 lightDir = normalize(vec3(6.5,0.5,-0.5) - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1.0f, 1.0f, 1.0f);;

	vec3 result = (ambient + diffuse) * var_color;
	FragColor = vec4(result, 1.0f);
};