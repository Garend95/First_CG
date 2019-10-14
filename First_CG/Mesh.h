#include "Buffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include <vector>

using namespace std;
#ifndef MESH_H
#define MESH_H

class Mesh{
    private:
        Buffer* buffer;
		VertexArray* VAO;
		Texture* tex;

    public:
		
		//creates and binds buffers, vertex arrays
        Mesh(vector<float> &coordinates);

		//returns the VAO for when we want to bind it
		VertexArray* getVAO(); 

		void assignPointersInVAO(bool colorsEnabled, bool normalsEnabled, bool texturesEnabled);

		void createTexture(const string imagePath, GLint Mode);

		void bindTexture();
};


#endif 