#include "Buffer.h"
#include "VertexArray.h"
#include <vector>

using namespace std;
#ifndef MESH2_H
#define MESH2_H

class Mesh2{
    private:
        Buffer* buffer;
		VertexArray* VAO;

    public:
		
		//creates and binds buffers, vertex arrays
        Mesh2(vector<float> &coordinates);

		//returns the VAO for when we want to bind it
		VertexArray* getVAO(); 

		void assignPointersInVAO(bool colorsEnabled, bool normalsEnabled, bool texturesEnabled);

};


#endif 