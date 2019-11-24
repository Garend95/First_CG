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
/*
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 8.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
*/
/*
class Camera {
	public:
		int id;
		glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 8.0f);
		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

		Camera(int id) {this->id = id);
		GetCamId() { return id };
};*/

struct globalCamera {
	glm::vec3 Pos = glm::vec3(0.0,0.0,0.0);
	glm::vec3 Front = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 Up = glm::vec3(0.0, 0.0, 0.0);

	void set(string type, glm::vec3 vector) {
		if (type == "Pos") Pos = vector;
		else if (type == "Front") Front = vector;
		else if (type == "Up") Up = vector;
	}

	glm::vec3 get(string type) {
		if (type == "Pos") return Pos;
		else if (type == "Front") return Front;
		else if (type == "Up") return Up;
	}
};

extern globalCamera globCam;

struct Material {
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float shininess;
};

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
/*
struct Cam {
	glm::vec3 
	glm::vec3 
	glm::vec3 
}*/
#endif 
