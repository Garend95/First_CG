#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;

out vec3 var_color;
uniform mat4 mvp;
uniform mat4 mv;

void main()
{
	var_color = aColor;
	gl_Position = mvp * vec4(aPos, 1);
};
