

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
#include "Global.h"
#include "Shadow.h"
globalCamera globCam;


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



vec3 cameraPos = vec3(0.0f, 0.0f, 8.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);



void processInput(GLFWwindow* window)
{

	float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
		
		globCam.set("Pos", cameraPos);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	globCam.set("Pos", cameraPos);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	globCam.set("Pos", cameraPos);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	globCam.set("Pos", cameraPos);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;
	globCam.set("Pos", cameraPos);
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;
	globCam.set("Pos", cameraPos);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		cameraFront = vec3(-cameraFront.x, cameraFront.y, -cameraFront.z);
	globCam.set("Front", cameraFront);
}

double lastX = 640;
double lastY = 400;
bool firstMouse = true;
float startingPitch = 0, startingYaw = 0;
bool starting = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float pitch = startingPitch, yaw = startingYaw;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = float(xpos - lastX);
	float yoffset = float(lastY - ypos);
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	startingPitch = pitch;
	startingYaw = yaw;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
	globCam.set("Front", cameraFront);
}



float lightAngle = 0;

int main(void)
{

	/*cameraPos = vec3(0.0f, 0.0f, 8.0f);
	cameraFront = vec3(0.0f, 0.0f, -1.0f);
	cameraUp = vec3(0.0f, 1.0f, 0.0f);*/


	GLFWwindow* window;
	globCam.set("Pos", cameraPos);
	globCam.set("Front", cameraFront);
	globCam.set("Up", cameraUp);

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1366, 768, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwSwapInterval(1);
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// call glewInit after creating the context...
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	/*
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	*/
	//glClearDepth(-1);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	//const char* vs = "model_loadingVS.shader";
	//const char* fs = "model_loadingFS.shader";
	Shader ourShader("C://Users//Garen//source//repos//First_CG//First_CG//Spotlight_VS.shader","C://Users//Garen//source//repos//First_CG//First_CG//Spotlight_FS.shader");
	Shader depthShader("C://Users//Garen//source//repos//First_CG//First_CG//depthShader_VS.shader", "C://Users//Garen//source//repos//First_CG//First_CG//depthShader_FS.shader");
	Shader renderShadowShader("C://Users//Garen//source//repos//First_CG//First_CG//modelAndShadow_VS.shader", "C://Users//Garen//source//repos//First_CG//First_CG//modelAndShadow_FS.shader");

	Shadow Sh(1024, 1024);
	Sh.setupDepthMap();

	//Shader ourShader("C://OpenGL//model_loadingVS.shader", "C://OpenGL//model_loadingFS.shader");
	//unsigned int program = ourShader.createShader(vs, fs);
	
	
	//string path = "Dependencies/Misc/nanosuit.obj";
	Model ourModel("C://Users//Garen//source//repos//First_CG//Dependencies//Misc//nanosuit.obj");
	//"C://OpenGL//Type_Writer.obj"

	mat4 Model = mat4(1.0f);
	mat4 View = lookAt(vec3(0, 0, 8),
		vec3(0, 0, -1), vec3(0, 1, 0));//center, lookat, up

	mat4 Projection = perspective(radians(60.0f),
		(float)4.0 / (float)3.0, 0.1f, 100.0f);

	Model = translate(Model, vec3(0.0f, 0.0f, -1.0f));
	//Model = glm::rotate(Model, 0.5236f, vec3(0, 1, 0));

	

	/*vec3 lightPos = vec3(-6.5, -10, -1);
	string vertexSource = ParseShader("vertex.shader");
	string fragmentSource = ParseShader("fragment.shader");

	unsigned int program = CreateShader(vertexSource, fragmentSource);

	string lightVertSource = ParseShader("lightVertex.shader");
	string lightFragSource = ParseShader("lightFragment.shader");*/

	
	//ourShader.use();
	

	float angle = 0;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{


		processInput(window);
		glfwSetCursorPosCallback(window, mouse_callback);

		vec3 p = globCam.get("Pos");
		vec3 f = globCam.get("Front");
		vec3 u = globCam.get("Up");
		View = glm::lookAt(p, p + f, u);

		//mat4 lightSpan = glm::rotate(Model,glm::radians(lightAngle),vec3(0,1,0));
		// Render here 
		glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//float distance = -0.1;
		//ourShader.use();

		mat4 mvp = Projection * View * Model;
		//mat4 mvpLight = Projection * View * Model * Model2;
		mat4 mv = View * Model;

		//glUseProgram(program);
		depthShader.use();
		//depthShader.setFloat("mvp", 1, mvp);
		depthShader.setFloat("model",1,Model);
		//depthShader.setFloat("view", 1, View);
		//depthShader.setFloat("viewPos", 1, p);
	
		// ourShader.setFloat("material.shininess", 128);

		// ourShader.setVector("light.position", 1, 2, 5, 1);
		// ourShader.setFloat("light.ambientStrength", 0.1);
		// ourShader.setFloat("light.diffuseStrength", 0.5);
		// ourShader.setFloat("light.specularStrength", 0.8);
		// ourShader.setFloat("light.constant", 1);
		// ourShader.setFloat("light.linear", 0.0);
		// ourShader.setFloat("light.quadratic", 0.032);

		//ourShader.setVector("light.position", 1, cameraPos.x, cameraPos.y, cameraPos.z);
    	//ourShader.setVector("light.direction", 1, cameraFront.x, cameraFront.y, cameraFront.z); 
   		//ourShader.setFloat("light.clipAngle", cos(glm::radians(12.5)));
    	//ourShader.setFloat("light.outerClipAngle", cos(glm::radians(17.5)));
		//ourShader.setFloat("mvp", 1, mvp);
		//glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(mvp));
		//glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_FALSE, value_ptr(mv));
		//glUniformMatrix4fv(worldSpace, 1, GL_FALSE, value_ptr(Model));
		// glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, value_ptr(cameraPos));
		// where worldSpace = glGetUniformLocation(program, "Model");
		vec3 lightPos = vec3(1.0f, 2.0f, 5.0f);
		Sh.renderToDepthMap(1.0,7.5,lightPos, depthShader);
		ourModel.Draw(depthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, 1366, 768); //switch viewport back to screen size  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderShadowShader.use();
		Sh.renderWithShadowMapping(1.0, 7.5,lightPos,renderShadowShader);
		ourModel.Draw(renderShadowShader);
		//Swap front and back buffers 
		glfwSwapBuffers(window);
		// Poll for and process events 
		glfwPollEvents();
	}


	//glDeleteProgram(program);

	glfwTerminate();
	return 0;
}