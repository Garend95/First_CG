
#ifndef MODEL_H
#define MODEL_H
//#define STB_IMAGE_IMPLEMENTATION


#include <GL/glew.h> // holds all OpenGL type declarations
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"
#include "modelMesh.h"
#include "Global.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;



class Model {
public:

	//Model(char* path)
	Model(string path);

	void Draw(Shader shader);

private:

	vector<modelMesh> meshes;
	string directory;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);

	modelMesh processMesh(aiMesh* mesh, const aiScene* scene);

	vector<Textr> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	vector<Material> loadMaterialProperties(aiMaterial* material);
	

	
};

unsigned int TextureFromFile(const char* path, const string& directory);

#endif