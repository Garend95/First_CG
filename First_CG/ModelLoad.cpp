

#include <GL/glew.h>
//#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Model.h"
#include "modelMesh.h"

using namespace std;
using namespace glm;

static string ParseShader(string filepath) {
	ifstream stream(filepath);
	string line;
	stringstream stringStream;

	while (getline(stream, line))
	{
		stringStream << line << '\n';
	}

	return stringStream.str();
}



// takes the shader codes as a string parameters 


//vec3 cameraPos = vec3(0.0f, 0.0f, 8.0f);
//vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
//vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
//
//void processInput(GLFWwindow* window)
//{
//
//	float cameraSpeed = 0.05f; // adjust accordingly
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//		cameraPos += cameraSpeed * cameraFront;
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//		cameraPos -= cameraSpeed * cameraFront;
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//		cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//		cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
//	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
//		cameraPos += cameraSpeed * cameraUp;
//	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
//		cameraPos -= cameraSpeed * cameraUp;
//	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
//		cameraFront = vec3(-cameraFront.x, cameraFront.y, -cameraFront.z);
//}
//
//double lastX = 640;
//double lastY = 400;
//bool firstMouse = true;
//float startingPitch = 0, startingYaw = 0;
//bool starting = true;
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	float pitch = startingPitch, yaw = startingYaw;
//
//	if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	float xoffset = float(xpos - lastX);
//	float yoffset = float(lastY - ypos);
//	lastX = xpos;
//	lastY = ypos;
//
//	float sensitivity = 0.1;
//	xoffset *= sensitivity;
//	yoffset *= sensitivity;
//
//	yaw += xoffset;
//	pitch += yoffset;
//
//	if (pitch > 89.0f)
//		pitch = 89.0f;
//	if (pitch < -89.0f)
//		pitch = -89.0f;
//
//	startingPitch = pitch;
//	startingYaw = yaw;
//
//	glm::vec3 front;
//	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//	front.y = sin(glm::radians(pitch));
//	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//	cameraFront = glm::normalize(front);
//}
//
//
//
//
//
//int main(void)
//{
//	GLFWwindow* window;
//
//	/* Initialize the library */
//	if (!glfwInit())
//		return -1;
//
//	/* Create a windowed mode window and its OpenGL context */
//	window = glfwCreateWindow(1280, 800, "Hello World", NULL, NULL);
//	if (!window)
//	{
//		glfwTerminate();
//		return -1;
//	}
//	glfwSwapInterval(1);
//	/* Make the window's context current */
//	glfwMakeContextCurrent(window);
//
//	// call glewInit after creating the context...
//	GLenum err = glewInit();
//	if (GLEW_OK != err)
//	{
//		/* Problem: glewInit failed, something is seriously wrong. */
//		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
//	}
//	/*
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CCW);
//	*/
//	//glClearDepth(-1);
//	glEnable(GL_DEPTH_TEST);
//	//glDepthFunc(GL_LESS);
//
//	//const char* vs = "model_loadingVS.shader";
//	//const char* fs = "model_loadingFS.shader";
//	Shader ourShader("C://OpenGL//model_loadingVS.shader", "C://OpenGL//model_loadingFS.shader");
//	//unsigned int program = ourShader.createShader(vs, fs);
//	
//	//string path = "Dependencies/Misc/nanosuit.obj";
//	Model ourModel("C://Users//Garen//source//repos//First_CG//Dependencies//Misc//nanosuit.obj");
//	//"C://OpenGL//Type_Writer.obj"
//
//	mat4 Model = mat4(1.0f);
//	mat4 View = lookAt(vec3(0, 0, 8),
//		vec3(0, 0, -1), vec3(0, 1, 0));//center, lookat, up
//
//	mat4 Projection = perspective(radians(60.0f),
//		(float)4.0 / (float)3.0, 0.1f, 100.0f);
//
//	Model = translate(Model, vec3(0.0f, 0.0f, -1.0f));
//	//Model = glm::rotate(Model, 0.5236f, vec3(0, 1, 0));
//
//	
//
//	/*vec3 lightPos = vec3(-6.5, -10, -1);
//	string vertexSource = ParseShader("vertex.shader");
//	string fragmentSource = ParseShader("fragment.shader");
//
//	unsigned int program = CreateShader(vertexSource, fragmentSource);
//
//	string lightVertSource = ParseShader("lightVertex.shader");
//	string lightFragSource = ParseShader("lightFragment.shader");*/
//
//	
//	
//
//	float angle = 0;
//	/* Loop until the user closes the window */
//	while (!glfwWindowShouldClose(window))
//	{
//
//
//		processInput(window);
//
//
//		glfwSetCursorPosCallback(window, mouse_callback);
//
//		View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//
//		// Render here 
//		glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		//float distance = -0.1;
//		//ourShader.use();
//
//		mat4 mvp = Projection * View * Model;
//		//mat4 mvpLight = Projection * View * Model * Model2;
//		mat4 mv = View * Model;
//
//		//glUseProgram(program);
//		ourShader.use();
//
//		ourShader.setFloat("mvp", 1, mvp);
//		//glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(mvp));
//		//glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_FALSE, value_ptr(mv));
//		//glUniformMatrix4fv(worldSpace, 1, GL_FALSE, value_ptr(Model));
//		// glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, value_ptr(cameraPos));
//		// where worldSpace = glGetUniformLocation(program, "Model");
//		ourModel.Draw(ourShader);
//
//
//		//Swap front and back buffers 
//		glfwSwapBuffers(window);
//		// Poll for and process events 
//		glfwPollEvents();
//	}
//
//
//	//glDeleteProgram(program);
//
//	glfwTerminate();
//	return 0;
//}