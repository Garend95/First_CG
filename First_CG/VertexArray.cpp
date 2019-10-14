#include "VertexArray.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

VertexArray::VertexArray() {
	this->VAO = new unsigned int[1];
}

void VertexArray::setVASize(int arraySize) {
	this->VAO = new unsigned int[arraySize];
}

void VertexArray::generateVAO(int numOfArrays, int index) {
	glGenVertexArrays(numOfArrays, &VAO[index]);
}

void VertexArray::bindVAO(int index) {
	glBindVertexArray(VAO[index]);
}

int VertexArray::sumOfDimensions(){ //Returns vertex array offset.
		return vertexSize + colorSize + normalSize + textureSize;
}

void VertexArray::assignPointers3D(bool colorsEnabled, bool NormalsEnabled, bool texturesEnabled) {
	if (colorsEnabled) this->colorSize = 3;
	if (NormalsEnabled) this->normalSize = 3;
	if (texturesEnabled) this->textureSize = 2;

	int offset = sumOfDimensions();

	//Vertex Coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(float) * offset, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	if (colorsEnabled) {
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(float) * offset, (GLvoid*)((vertexSize) * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	if (NormalsEnabled) {
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(float) * offset, (GLvoid*)((vertexSize + colorSize) * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	if (texturesEnabled) {
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(float) * offset, (GLvoid*)((vertexSize + colorSize + normalSize) * sizeof(float)));
		glEnableVertexAttribArray(3);
	}
}

VertexArray* VertexArray::get() {
	return this;
}

