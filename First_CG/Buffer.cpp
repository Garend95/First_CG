#include "Buffer.h"
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

Buffer::Buffer() {
	this->buffer = new unsigned int[1];
}

void Buffer::setSize(int arraySize){
	this->buffer = new unsigned int[arraySize];
}

void Buffer::generateBuffers(int numberOfBuffers) {
	glGenBuffers(numberOfBuffers, buffer);
}

void Buffer::bindBuffer(int index) {
	glBindBuffer(GL_ARRAY_BUFFER, buffer[index]);
}

void Buffer::provideBufferData(vector<float> &coordinates, GLint Mode) {
	glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(float), &coordinates[0], Mode);
}

unsigned int Buffer::getBuffer() {
	return *buffer;
}