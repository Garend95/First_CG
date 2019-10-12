#ifndef BUFFER_H
#define BUFFER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Buffer
{
private:
    unsigned int *buffer;

public:

    //constructor is empty
	Buffer();

    //we decide the size of the buffer for the object upon initialization
    void setSize(int arraySize); 

    //number of buffers to be generated
	void generateBuffers(int numberOfBuffers);

    //binds the buffer at the current to the GPU
	void bindBuffer(int index); 
    
    //mode for data input is currently fixed
	void provideBufferData(float* coordinates, int size, GLint Mode); 
    
};





#endif