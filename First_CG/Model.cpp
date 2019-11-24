#include "Model.h"
#include "modelMesh.h"

#include <GL/glew.h> // holds all OpenGL type declarations
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Global.h"
using namespace std;




Model::Model(string path){
    loadModel(path);
}

void Model::Draw(Shader shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++) { //will draw each mesh, wehere meshes is a vector of modelMesh
        meshes[i].Draw(shader); //seperate draw function implemented in mesh class definition
    }
};

void Model::loadModel(string path){ //loads from path, checks for errors, if all is ok, we will process the root node
    Assimp::Importer import;                                                                 //imported scene will have right handed coordinate system (Z points away from us, Y is up)
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); //triangulate the faces if they are not already
                                                                                             //flip the UVs as they are mostly loaded with negative y values
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	//chooses directory as whatever comes after the last /
	directory = path.substr(0, path.find_last_of('/'));


	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene){
    for (unsigned int i = 0; i < node->mNumMeshes; i++) //for each node that was accessed, we process that node's meshes before 
	{                                                   //calling and processing the children of these nodes
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; //node->mMeshes[i] stores the index for the i'th mesh
		meshes.push_back(processMesh(mesh, scene)); //process the ith mesh and push it into the mesh vector which we later draw one vector member after the other
	}

	//process children of the ith mesh
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

modelMesh Model::processMesh(aiMesh* mesh, const aiScene* scene){
    vector<Vertex> vertices; //for each mesh about to be processed, we create a vertex struct instance,
    //we cycle through each vertex of the mesh, and assign positions, normals and texture information to the instance before finally 
    //pushing that instance to the vector of vertices 
    vector<unsigned int> indices;
    vector<Textr> textures;
	vector<Material> properties;
     

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex; //our own placeholder vertex to ensure appropriate conversion 
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);


    }

    //here we need to push the indicies of each face of the mesh into a vector
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }


    if (mesh->mMaterialIndex >= 0) //we need to know that an index exists?
    {                              //ai scene is universal
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; //so mesh->mMaterialIndex provides the index of the mesh's materials
        
        vector<Textr> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse"); //this loads the allotted defuse texture only. material properties need to be retrieved elsewhere.
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        vector<Textr> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
   
        vector<Material> props = loadMaterialProperties(material);
		properties.insert(properties.end(), props.begin(), props.end());
        
		
    }

    return modelMesh(vertices, indices, textures, properties);
}


vector<Textr> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
    vector<Textr> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str; //empty path
        mat->GetTexture(type, i, &str); //set texture i of type 'type's path to the string address
        

        Textr texture; //
        texture.id = TextureFromFile(str.C_Str(), directory);
        texture.type = typeName;
        texture.path = str;
        
        textures.push_back(texture);
    }
    return textures;
};

vector<Material> Model::loadMaterialProperties(aiMaterial* material) {
    
	vector<Material> properties;

    aiColor4D Ka; 
    aiColor4D Kd;
    aiColor4D Ks;
    float shininess;

	aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &Ka);
    aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &Kd);
    aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &Ks);
    aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);
    
    Material mat;
    mat.Ambient = glm::vec3(Ka.r,Ka.g,Ka.b);
    mat.Diffuse = glm::vec3(Kd.r,Kd.g,Kd.b);
    mat.Specular = glm::vec3(Ks.r,Ks.g,Ks.b);
	mat.shininess = shininess;

	properties.push_back(mat);

	return properties;
}

unsigned int TextureFromFile(const char* path, const string& directory)
{
	string filename = string(path);
	filename = directory + '/' + filename; //requires absolute path

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
};