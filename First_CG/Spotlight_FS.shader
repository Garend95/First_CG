#version 330 core


out vec4 FragColor;
in vec3 FragPos;
//in vec3 var_color;
in vec3 Normals;
in vec2 TexCoord;

//in vec3 gl_position
//uniform vec4 u_Color;
//vec3 lightPos = v(6.5, 0.5, -0.5);




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

struct Property {
	float Ka;
	float Kd;
	float Ks;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;


//uniform Material material;
uniform Property prop;
uniform float shininess;
uniform Light light;
uniform vec3 viewPos;
uniform mat4 lightSpan;
uniform mat4 lightTranslate;

//sending in our texture
 //Unused

void main()
{
	
	vec4 ambient = light.ambientStrength * prop.Ka * texture(texture_diffuse1, TexCoord);

	vec3 norm = normalize(Normals);

	//mat4 offCenter = glm::translate(mat4(1.0), vec3(7, 0, 0));
	//our light position is at the same location of the camera
	vec4 lightSource = vec4(vec3((light.position).x, (light.position).y, (light.position).z), 1) * lightSpan;
	
	vec3 lightDir = normalize(vec3(lightSource) - FragPos);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	//diffuse lighting
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = light.diffuseStrength * diff * prop.Kd * texture(texture_diffuse1,TexCoord); //vec3(prop.Kd, prop.Kd, prop.Kd) ;
	


	//specular lighting (without halfway vector), swap halfwayDir below with reflectDir
	//vec3 viewDir = normalize(viewPos - FragPos);
	//vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
	vec4 specular = light.specularStrength * spec * prop.Ks * texture(texture_specular1,TexCoord); //vec3(prop.Ks, prop.Ks, prop.Ks) ;


	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	//vec3 result = (ambient + diffuse + specular) * var_color;
	//FragColor = vec4(result, 1.0);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.clipAngle - light.outerClipAngle;
	float intensity = clamp((theta - light.outerClipAngle) / epsilon, 0.0, 1.0);

	diffuse *= intensity;
	specular *= intensity;
	//vec4 result =  /* vec4(var_color, 1)*/;

	//if(theta > light.clipAngle){
		/*vec4((ambient + diffuse + specular), 1) +*/
		FragColor =  ambient + diffuse + specular;
	//}
	//else
	//FragColor = vec4(ambient * vec3(texture(material.diffuse, TexCoord)), 1.0);
};