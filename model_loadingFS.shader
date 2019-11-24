#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

//why is the model loading texture bound this way, why is it not listed as sampler 2D
uniform sampler2D texture_diffuse1; //this is supposed to be our texture
//somehow this texture is a uniform sampler 2D 

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
}