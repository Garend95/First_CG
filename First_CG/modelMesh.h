#ifndef MODELMESH_H
#define MODELMESH_H

#include <vector>
#include <iostream>
#include "Shader.h"
#include "Global.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;


class modelMesh {
    public:
        /*  Mesh Data  */
        vector<Vertex> vertices;
        vector<unsigned int> indices;
    	vector<Texture> textures;
	
        /*  Functions  */
        modelMesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

        void Draw(Shader shader);
    private:
        /*  Render data  */
	    unsigned int VAO, VBO, EBO;

        void setupMesh();
};
#endif