#version 330 core 

out vec4 FragColor;
in vec3 var_color;

//uniform vec4 u_Color;

void main()
{
	
	FragColor = vec4(var_color,1);
	
};