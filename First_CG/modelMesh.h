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
    	vector<Textr> textures;
		vector<Material> materials;

		glm::vec3 center;
	
        /*  Functions  */
        modelMesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Textr> textures, vector<Material> materials);

        void Draw(Shader shader);

		glm::vec3 getCenter();
    private:
        /*  Render data  */
	    unsigned int VAO, VBO, EBO;

		

        void setupMesh();
};
#endif