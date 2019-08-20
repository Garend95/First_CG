#version 330 core 

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

out vec3 FragPos;
out vec3 CubeNormal;


out vec3 var_color;
uniform mat4 mvp;
//uniform mat4 mv;
uniform mat4 Model;


void main()
{
	var_color = color;
	gl_Position =  mvp * vec4(position,1);
	FragPos = vec3(Model * vec4(position, 1));
	CubeNormal = normal;
	
};
