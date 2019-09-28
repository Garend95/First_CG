#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;

//out vec4 FragPos;
out vec4 gl_Position;

out vec3 var_color;
//out vec3 Normal;
//uniform mat4 mvp;
//uniform mat4 mv;
//currently model 2 has replaced the 
uniform mat4 mvpLight;


void main()
{
	var_color = aColor;
	gl_Position = mvpLight * vec4(aPos, 1);
	//FragPos = vec3(Model * vec4(aPos, 1));
	Normal = aNormal;
};
