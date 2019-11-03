#version 330 core

out vec4 FragColor;
in vec3 FragPos;
in vec3 var_color;
in vec3 CubeNormal;
in vec2 TexCoord;

//in vec3 gl_position
//uniform vec4 u_Color;
//vec3 lightPos = v(6.5, 0.5, -0.5);


struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;
	vec3 pivot;

	float ambientStrength;
	float diffuseStrength;
	float specularStrength;

	float constant;
	float linear;
	float quadratic;

	float clipAngle;
	float outerClipAngle;

};


uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform mat4 lightSpan;
uniform mat4 lightTranslate;

//sending in our texture
uniform sampler2D ourTexture;

void main()
{

	vec3 ambient = light.ambientStrength * vec3(texture(material.diffuse, TexCoord));

	vec3 norm = normalize(CubeNormal);

	//mat4 offCenter = glm::translate(mat4(1.0), vec3(7, 0, 0));
	vec4 lightSource = vec4(vec3((light.position).x, (light.position).y, (light.position).z), 1) * lightSpan;
	
	vec3 lightDir = normalize(vec3(lightSource) - FragPos);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	//diffuse lighting
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuseStrength * diff * vec3(texture(material.diffuse, TexCoord));



	//specular lighting (without halfway vector), swap halfwayDir below with reflectDir
	/*vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);*/

	float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
	vec3 specular = light.specularStrength * spec * vec3(texture(material.specular, TexCoord));


	/*float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;*/

	//vec3 result = (ambient + diffuse + specular) * var_color;
	//FragColor = vec4(result, 1.0);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.clipAngle - light.outerClipAngle;
	float intensity = clamp((theta - light.outerClipAngle) / epsilon, 0.0, 1.0);

	diffuse *= intensity;
	specular *= intensity;
	//vec4 result =  /* vec4(var_color, 1)*/;

	//if(theta > light.clipAngle){
		FragColor = vec4((ambient + diffuse + specular), 1);
	//}
	//else
	//FragColor = vec4(ambient * vec3(texture(material.diffuse, TexCoord)), 1.0);
};