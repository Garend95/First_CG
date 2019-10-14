#include "Texture.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
Texture::Texture(){
    this->width = 0;
    this->height = 0;
    this->nrchannels = 0;
}

void Texture::generateAndBindTexture2D(GLint GL_Mode) {
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_Mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_Mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_Mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_Mode);
}

void Texture::loadTexture(const string imagePath){
		data = stbi_load(imagePath.c_str(), &width, &height, &nrchannels, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			cout << "failed to load texture" << endl;
		}

}

void Texture::bindTexture2D(){
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::freeImage(){
    stbi_image_free(data);
}