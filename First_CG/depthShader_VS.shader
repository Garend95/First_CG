#version 330 core
//#include <iostream>

layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 normal;
//layout(location = 2) in vec2 aTexCoord;
//std::cout << "hello" << std::endl;
//out vec3 FragPos;
//out vec3 Normals;
//out vec2 TexCoord;
//out vec4 gl_position;

//uniform mat4 mvp;
uniform mat4 model;
//uniform mat4 view;
uniform mat4 lightSpaceMatrix;
//uniform mat4 viewSpaceMatrix;
//vec4 first;

void main()
{
	//first = model * vec4(position, 1);
	//gl_Position =  first;
	gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f); //* vec4(position, 1);
	//FragPos = vec3(model * vec4(position, 1));
	//Normals = normal;
	//TexCoord = aTexCoord;
}
