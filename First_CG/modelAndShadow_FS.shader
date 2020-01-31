#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normals;
in vec2 TexCoords;
in vec4 FragPosLightSpace;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
//uniform sampler2D depthMap;
//uniform sampler2D shadowMap;


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


uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace) {
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	projCoords = projCoords * 0.5 + 0.5; //convert NDC coordinates to [0,1]

	//the parameters take sampler2dshadows and a 3 sized vector that contains texture coordinates
	//float closestDepth = texture(shadowMap, projCoords.xy).r; //what info is stored in the red values of the vector for us to ask about rgb

	//float currentDepth = projCoords.z;

	//float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

	//return shadow;
	return 0;
}

/*
uniform Property prop;
uniform float shininess;
uniform Light light;
uniform vec3 viewPos; //
uniform mat4 lightTranslate; //matrix for translating light source

*/

void main()
{
	vec3 color = texture(texture_diffuse1, TexCoords).rgb;
	vec3 normal = normalize(Normals);
	vec3 lightColor = vec3(1.0);

	vec3 ambient = 0.15 * color;

	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - FragPos);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	//float shadow = ShadowCalculation(FragPosLightSpace);
	vec3 lighting = (ambient + /*(1.0 - shadow) * (*/diffuse + specular)/*)*/ * color;

	FragColor = vec4(lighting, 1.0);

	/*
		float depthValue = texture(depthMap, TexCoords).r;
		FragColor = vec4(vec3(depthValue), 1.0);
	*/

	
}