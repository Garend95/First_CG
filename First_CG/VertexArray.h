#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VertexArray
{
private:
    unsigned int *VAO;

    //These define the number of dimensions that each property will have
    unsigned int vertexSize = 3;
    unsigned int colorSize = 0;
    unsigned int normalSize = 0;
    unsigned int textureSize = 0;


public:
	VertexArray(); 

    void setVASize(int arraySize);

	void generateVAO(int numOfArrays, int index);

	void bindVAO(int index);

	void assignPointers3D(bool colorsEnabled, bool NormalsEnabled, bool texturesEnabled);
    
	VertexArray* get();

	int sumOfDimensions();
};




#endif