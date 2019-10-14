#ifndef TEXTURE_H
#define TEXTURE_H	
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <filesystem>
#include <iostream>
#include <string>


using namespace std;

class Texture {
private:
	int width;
	int height;
	int nrchannels;
	unsigned int texture;
	unsigned char* data;

public:
	Texture();

	void generateAndBindTexture2D(GLint GL_Mode);

	void loadTexture(const string imagePath);

    void bindTexture2D();

	void freeImage();

};
#endif