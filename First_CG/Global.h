#ifndef GLOBAL_H
#define GLOBAL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>
#include <vector>

/*
double lastX = 640;
double lastY = 400;
bool firstMouse = true;
float startingPitch = 0, startingYaw = 0;
bool starting = true;

vec3 cameraPos = vec3(0.0f, 0.0f, 8.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
*/

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Textr {
	unsigned int id;
	std::string type;
	aiString path;
};

#endif 
