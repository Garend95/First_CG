#version 330 core 

out vec4 FragColor;
in vec3 FragPos;
in vec3 var_color;
in vec3 CubeNormal;
in vec2 TexCoord;

//in vec3 gl_position
//uniform vec4 u_Color;
//vec3 lightPos = v(6.5, 0.5, -0.5);


uniform vec3 viewPos;

//sending in our texture
uniform sampler2D ourTexture;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);

	vec3 norm = normalize(CubeNormal);
	vec4 lightSource = vec4(3, 6, 3, 1);
	vec3 lightDir = normalize(vec3(lightSource) - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(0.0f, 1.0f, 1.0f);;

	float specularStrength = 0.1;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0), 32);
	vec3 specular = specularStrength * spec * vec3(1.0f, 0.0f, 1.0f);

	/*texture(ourTexture, TexCoord)*/
	//vec3 result = (ambient + diffuse + specular) * var_color;
	//FragColor = vec4(result, 1.0);

	vec4 result = vec4((ambient + diffuse + specular), 1) * texture(ourTexture, TexCoord) /** vec4(var_color, 1)*/;
	FragColor = result;
};