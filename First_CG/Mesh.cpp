#include "Mesh2.h"
#include "VertexArray.h"
#include <vector>

using namespace std;

Mesh2::Mesh2(vector<float> &coordinates) {
	buffer = new Buffer();
	VAO = new VertexArray();

	(*buffer).setSize(1);
	(*buffer).generateBuffers(1);
	(*buffer).bindBuffer(0);
	(*buffer).provideBufferData(coordinates, GL_STATIC_DRAW);

	(*VAO).setVASize(1);
	(*VAO).generateVAO(1, 0);
	(*VAO).bindVAO(0);
	
}

void Mesh2::assignPointersInVAO(bool colorsEnabled, bool normalsEnabled, bool texturesEnabled) {
	(*VAO).assignPointers3D(colorsEnabled, normalsEnabled, texturesEnabled);
}

VertexArray* Mesh2::getVAO() {
	return (*VAO).get();
}