#include "Mesh.h"
#include "VertexArray.h"
#include "Texture.h"
#include <vector>

using namespace std;

Mesh::Mesh(vector<float> &coordinates) {
	buffer = new Buffer();
	VAO = new VertexArray();

	(*buffer).setSize(1);
	(*buffer).generateBuffers(1);
	(*buffer).bindBuffer(0); // <- binds to chosen index
	(*buffer).provideBufferData(coordinates, GL_STATIC_DRAW); // <- 

	(*VAO).setVASize(1);
	(*VAO).generateVAO(1, 0);
	(*VAO).bindVAO(0);
	
}

VertexArray* Mesh::getVAO() {
	return (*VAO).get();
}

void Mesh::assignPointersInVAO(bool colorsEnabled, bool normalsEnabled, bool texturesEnabled) {
	(*VAO).assignPointers3D(colorsEnabled, normalsEnabled, texturesEnabled);
}

/*
void Mesh::createTexture(const string imagePath, GLint Mode) {
	
	tex = new Texture();
	(*tex).generateAndBindTexture2D(Mode);
	(*Texture).loadTexture(imagePath);
	(*tex).freeImage();
	
}

void Mesh::bindTexture() {
	(*tex).bindTexture2D();
}
*/


void Mesh::createDiffuseTexture(const string imagePath, GLint Mode) {
	
	diffuseMap = new Texture();
	(*diffuseMap).generateAndBindTexture2D(Mode);
	(*diffuseMap).loadTexture(imagePath);
	(*diffuseMap).freeImage();
	
}

void Mesh::bindDiffuseTexture() {
	(*diffuseMap).bindTexture2D();
}

void Mesh::createSpecularTexture(const string imagePath, GLint Mode) {
	
	specularMap = new Texture();
	(*specularMap).generateAndBindTexture2D(Mode);
	(*specularMap).loadTexture(imagePath);
	(*specularMap).freeImage();
	
}

void Mesh::bindSpecularTexture() {
	(*specularMap).bindTexture2D();
}