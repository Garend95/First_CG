#include "modelMesh.h"
#include "Global.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
//globalCamera globCam;

modelMesh::modelMesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Textr> textures, vector<Material> materials) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
	this->materials = materials;
    setupMesh();
}

//this function is called iteratively during the process of 
void modelMesh::Draw(Shader shader) {
    unsigned int diffuseNr = 1; //
    unsigned int specularNr = 1; // 
    //shader.use();
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        string number;
        string name = textures[i].type; //we cannot have the texture type of an index be both a diffuse and specular
        if (name == "texture_diffuse")
        {
            number = to_string(diffuseNr++);
            //shader.setInt("material.diffuse", i);
			//shader.setFloat(("material." + name + number)
        }
            
        else if (name == "texture_specular")
        {
            number = std::to_string(specularNr++);
            //shader.setInt("material.specular", i);
        }
            
        //need to set diffuse and specular

		//
        shader.setFloat(("material." + name + number).c_str(), i);

        //unassigned id of texture is the address of assignment, hence we can have as many addresses as maximum of 'i'
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    

    /*here let us place some of the 
    
    */
    //shader.setVector()
    
    /*shader.setVector("property.Ka", 1, materials[0].Ambient.x, materials[0].Ambient.y, materials[0].Ambient.z);
	shader.setVector("property.Kd", 1, materials[0].Diffuse.x, materials[0].Diffuse.y, materials[0].Diffuse.z);
	shader.setVector("property.Ks", 1, materials[0].Specular.x, materials[0].Specular.y, materials[0].Specular.z);*/

    //We need only pass the x value of the vectors as the y and z would have the exact same value
    shader.setFloat("prop.Ka",materials[0].Ambient.x);
    shader.setFloat("prop.Kd",materials[0].Diffuse.x);
    shader.setFloat("prop.Ks",materials[0].Specular.x);
    shader.setFloat("shininess",materials[0].shininess);

  
		
	//shader.setFloat("material.shininess", 128);

	//shader.setVector("light.position", 1, 2, 5, 1);
	shader.setFloat("light.ambientStrength", 0.1);
	shader.setFloat("light.diffuseStrength", 1);
	shader.setFloat("light.specularStrength", 0.8);
	shader.setFloat("light.constant", 1);
	shader.setFloat("light.linear", 0.0);
	shader.setFloat("light.quadratic", 0.032);
    
	glm::vec3 p = globCam.get("Pos");
	glm::vec3 f = globCam.get("Front");
	//glm::vec3 u = globCam.get("Up");

    shader.setVector("light.position", 1, p.x, p.y, p.z);
    shader.setVector("light.direction", 1, 1, -1, 0);
   
	glActiveTexture(GL_TEXTURE0); //is this ever a bad idea
	/*
   	shader.setFloat("light.clipAngle", cos(glm::radians(12.5)));
    shader.setFloat("light.outerClipAngle", cos(glm::radians(17.5)));
    */
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    //new line, do we need to unbind this? glBindBuffer(GL_ARRAY_BUFFER, 0)
    glBindVertexArray(0);
}

glm::vec3 modelMesh::getCenter() {
	float centerX = 0, 
		  centerY = 0, 
		  centerZ = 0;

	int size = vertices.size();

    float maxX = vertices[0].Position.x, minX = vertices[0].Position.x,
          maxY = vertices[0].Position.y, minY = vertices[0].Position.y,
          maxZ = vertices[0].Position.z, minZ = vertices[0].Position.z;

	for (int i = 0; i < size; i++) {
		if (maxX < vertices[i].Position.x) maxX = vertices[i].Position.x;
        else if (minX > vertices[i].Position.x) minX = vertices[i].Position.x;

        if (maxY < vertices[i].Position.y) maxY = vertices[i].Position.y;
        else if (minY > vertices[i].Position.y) minY = vertices[i].Position.y;

        if (maxZ < vertices[i].Position.z) maxZ = vertices[i].Position.z;
        else if (minZ > vertices[i].Position.z) minZ = vertices[i].Position.z;
	}
	
	centerX = (minX + maxX)/2;
	centerY = (minY + maxY)/2;
	centerZ = (minZ + maxZ)/2;

	return glm::vec3(centerX, centerY, centerZ);

}

void modelMesh::setupMesh(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
     
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //This serves as an index indicator to the variety of vertecies needed k
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); //borrows normal property from the Vertex struct for offsetting
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}