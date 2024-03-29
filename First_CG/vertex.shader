#version 330 core 

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 CubeNormal;
out vec2 TexCoord;

out vec3 var_color;
uniform mat4 mvp;
//uniform mat4 mv;
uniform mat4 model;
uniform mat4 view;


void main()
{
	var_color = color;
	gl_Position =  mvp * vec4(position,1);
	FragPos = vec3(model * vec4(position, 1));
	CubeNormal = normal;
	TexCoord = aTexCoord;
	
};
