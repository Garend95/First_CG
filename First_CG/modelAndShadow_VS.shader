#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normals;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 view;
uniform mat4 viewSpaceMatrix; //Passes both the light space and view space matricies 
							   //Here we pass the view space matrix
uniform mat4 lightSpaceMatrix;

void main()
{
	FragPos = vec3(model * vec4(position, 1));
	Normals = transpose(inverse(mat3(model))) * normal; //why are normals transposed ?*?
	TexCoords = aTexCoord;
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	gl_Position = mvp * vec4(position, 1);
}
