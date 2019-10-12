#include "Buffer.h"
#include "VertexArray.h"

#ifndef MESH2_H
#define MESH2_H

class Mesh2{
    private:
        Buffer* buffer;
		VertexArray* VAO;

    public:// we cannot place a get method in a class to which the gotten data does not belong
		VertexArray* getVAO() {
			return (*VAO).get();
		}

		/*
		unsigned int* getVAO() {
			return (*VAO).get();
		}
		*/

        Mesh2(float coordinates[], int sizeOfCoordinates){
			buffer = new Buffer();
			VAO = new VertexArray();

			(*buffer).setSize(1);
			(*buffer).generateBuffers(1);
			(*buffer).bindBuffer(0);
			(*buffer).provideBufferData(coordinates, sizeOfCoordinates, GL_STATIC_DRAW);

			(*VAO).setVASize(1);
			(*VAO).generateVAO(1, 0);
			(*VAO).bindVAO(0);
			(*VAO).assignPointers3D(true, true, false);
        };


};


#endif 