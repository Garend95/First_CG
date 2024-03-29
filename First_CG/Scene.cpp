//#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
//#include <glad/glad.h>
//#include "stb_image.h"
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

#include "Buffer.h"
#include "VertexArray.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Global.h"

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

static unsigned int CompileShader(unsigned int type, const string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		cout << message << endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader)
{
	GLuint program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);


	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program); // validate if the program is valid and can be run in the current state of opengl

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
/*
vec3 cameraPos = vec3(0.0f, 0.0f, 8.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
*/
//float lightAngle = 0;
float lightPosition = 0;

struct light { //These positions are not for initializing the bulb but for the translation changes in position as keys are pressed
	float positionX = 0;
	float positionY = 0;
	float positionZ = 0;
	glm::vec3 centerOfInteraction; //not yet integrated
} bulb;

struct material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

float positionX = 0;
float positionY = 0;
float positionZ = 0;
mat4 lightTranslate;
/*
void processInput(GLFWwindow* window)
{
	
	float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_SPACE ) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		cameraFront = vec3(-cameraFront.x, cameraFront.y, -cameraFront.z);
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		cameraFront = vec3(cameraFront.x, -cameraFront.y, cameraFront.z);
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		lightAngle += 0.2;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		lightAngle -= 0.2;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		lightTranslate = glm::translate(mat4(1.0), vec3(0, 0.2, 0));
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		lightTranslate = glm::translate(mat4(1.0), vec3(0, -0.2, 0));
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		lightTranslate = glm::translate(mat4(1.0), vec3(-0.2, 0, 0));
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		lightTranslate = glm::translate(mat4(1.0), vec3(0.2, 0, 0));
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		lightTranslate = glm::translate(mat4(1.0), vec3(0, 0, 0.2));
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		lightTranslate = glm::translate(mat4(1.0), vec3(0, 0, -0.2));
}
*/

/*
double lastX = 640;
double lastY = 400;
bool firstMouse = true;
float startingPitch = 0, startingYaw = 0;
bool starting = true;
*/

/*
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float pitch = startingPitch, yaw = startingYaw;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = 2.0*float(xpos - lastX);
	float yoffset = 2.0 * float(lastY - ypos);
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
}
*/




//int main(void)
//{
//
//	
//
//	Window window(1280, 800);
//	if (window.windowHasError()) return -1;
//
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CCW);
//
//	//glClearDepth(-1);
//	glEnable(GL_DEPTH_TEST);
//	//glDepthFunc(GL_LESS);
//	
//	
//	vector<float> coordinates = {
//		0,0,0,  255,255,0, 0,0,1,      0, 0,
//		4,4,0, 	255,255,0, 0,0,1,      1, 1,
//		0,4,0, 	255,255,0, 0,0,1,      0, 1,
//		0,0,0, 	255,255,0, 0,0,1,      0, 0,
//		4,0,0, 	255,255,0, 0,0,1,      1, 0,
//		4,4,0, 	255,255,0, 0,0,1,      1, 1,
//		4,0,0, 	255,255,0, 1,0,0,       0, 0,
//		4,4,-4,	 255,255,0, 1,0,0,      0, 0,
//		4,4,0, 	255,255,0, 1,0,0,       0, 0,
//		4,0,0, 	255,255,0, 1,0,0,       0, 0,
//		4,0,-4,	 255,255,0, 1,0,0,      0, 0,
//		4,4,-4,	 255,255,0, 1,0,0,      0, 0,
//		0,0,-4,	 255,255,0, 0,0,-1,     0, 0,
//		4,4,-4,	 255,255,0, 0,0,-1,     0, 0,
//		4,0,-4,	 255,255,0, 0,0,-1,     0, 0,
//		0,0,-4,	 255,255,0, 0,0,-1,     0, 0,
//		0,4,-4,	 255,255,0, 0,0,-1,     0, 0,
//		4,4,-4,	 255,255,0, 0,0,-1,     0, 0,
//		0,0,0, 	255,255,0, -1,0,0,      0, 0,
//		0,4,-4,	 255,255,0, -1,0,0,     0, 0,
//		0,0,-4,	 255,255,0, -1,0,0,     0, 0,
//		0,0,0, 	255,255,0, -1,0,0,      0, 0,
//		0,4,0, 	255,255,0,  -1,0,0,     0, 0,
//		0,4,-4,	 255,255,0, -1,0,0,     0, 0,
//		4,4,0, 	255,255,0,   0,1,0,     1, 0,
//		4,4,-4,	 255,255,0,  0,1,0,     1, 1,
//		0,4,-4,	 255,255,0,  0,1,0,     0, 1,
//		4,4,0, 	255,255,0,   0,1,0,     1, 0,
//		0,4,-4,	 255,255,0,  0,1,0,     0, 1,
//		0,4,0,	255,255,0,   0,1,0,     0, 0,
//		0,0,-4, 255,255,0,   0,-1,0,    0, 0,
//		4,0,-4,	 255,255,0,  0,-1,0,    0, 0,
//		4,0,0,	 255,255,0,  0,-1,0,    0, 0,
//		0,0,0, 	255,255,0,   0,-1,0,    0, 0,
//		0,0,-4,	 255,255,0,  0,-1,0,    0, 0,
//		4,0,0,	255,255,0,   0,-1,0,    0, 0
//
//	};
//
//	vector <float> floor = {
//		-10,0,10, 0,1,0, 0,0,
//		10,0,-10, 0,1,0, 0,0,
//		-10,0,-10, 0,1,0, 0,0,
//		-10,0,10, 0,1,0, 0,0,
//		10,0,10, 0,1,0, 0,0,
//		10,0,-10, 0,1,0, 0,0
//		
//
//	};
//
//	vector<float> lightcoordinates = {
//		7,0,0,  0,0,255, 0,0,1,    0, 0,
//		7,1,0,  0,0,255, 0,0,1,	   1, 1,
//		6,1,0,  0,0,255, 0,0,1,	   0, 1,
//		7,0,0,  0,0,255, 0,0,1,	   0, 0,
//		6,1,0,  0,0,255, 0,0,1,	   1, 0,
//		6,0,0,  0,0,255, 0,0,1,	   1, 1,
//		7,0,0,  0,0,255, 1,0,0,	    0, 0,
//		7,1,-1,	0,0,255,  1,0,0,    0, 0,
//		7,1,0,	0,0,255, 1,0,0,	    0, 0,
//		7,0,0,	0,0,255, 1,0,0,	    0, 0,
//		7,0,-1,	0,0,255,  1,0,0,    0, 0,
//		7,1,-1,	0,0,255,  1,0,0,    0, 0,
//		6,0,-1, 0,0,255,  0,0,-1,   0, 0,
//		7,1,-1,	0,0,255,  0,0,-1,   0, 0,
//		7,0,-1,	0,0,255,  0,0,-1,   0, 0,
//		6,0,-1,	0,0,255,  0,0,-1,   0, 0,
//		6,1,-1,	0,0,255,  0,0,-1,   0, 0,
//		7,1,-1,	0,0,255,  0,0,-1,   0, 0,
//		6,0,0,  0,0,255, -1,0,0,    0, 0,
//		6,1,-1,	0,0,255,  -1,0,0,   0, 0,
//		6,0,-1,	0,0,255,  -1,0,0,   0, 0,
//		6,0,0,	0,0,255, -1,0,0,    0, 0,
//		6,1,0,	0,0,255,  -1,0,0,   0, 0,
//		6,1,-1,	0,0,255,  -1,0,0,   0, 0,
//		7,1,0,  0,0,255,   0,1,0,   1, 0,
//		7,1,-1,	0,0,255,   0,1,0,   1, 1,
//		6,1,-1,	0,0,255,   0,1,0,   0, 1,
//		7,1,0,	0,0,255,   0,1,0,   1, 0,
//		6,1,-1,	0,0,255,   0,1,0,   0, 1,
//		6,1,0,	0,0,255,   0,1,0,   0, 0,
//		6,0,-1, 0,0,255,   0,-1,0,  0, 0,
//		7,0,-1,	0,0,255,   0,-1,0,  0, 0,
//		7,0,0,	0,0,255,   0,-1,0,  0, 0,
//		6,0,0,	0,0,255,   0,-1,0,  0, 0,
//		6,0,-1,	0,0,255,   0,-1,0,  0, 0,
//		7,0,0,	0,0,255,   0,-1,0,  0, 0
//	};
//
//	Mesh Cube(coordinates);
//	Cube.assignPointersInVAO(true, true, true);
//	Cube.createDiffuseTexture("C://Users//Garen//source//repos//First_CG//Dependencies//Misc//container2.png", GL_REPEAT);
//	Cube.createSpecularTexture("C://Users//Garen//source//repos//First_CG//Dependencies//Misc//container2_specular.png", GL_REPEAT);
//
//	Mesh LightCube(lightcoordinates);
//	LightCube.assignPointersInVAO(true, true, true);
//	LightCube.createDiffuseTexture("C://Users//Garen//source//repos//First_CG//Dependencies//Misc//container2.png", GL_REPEAT);
//	LightCube.createSpecularTexture("C://Users//Garen//source//repos//First_CG//Dependencies//Misc//container2_specular.png", GL_REPEAT);
//
//	Mesh Floor(floor);
//	Floor.assignPointersInVAO(false,true,true);
//	Floor.createDiffuseTexture("C://Users//Garen//source//repos//First_CG//Dependencies//Misc//container2.png", GL_REPEAT);
//	Floor.createSpecularTexture("C://Users//Garen//source//repos//First_CG//Dependencies//Misc//container2_specular.png", GL_REPEAT);
//
//	mat4 model = mat4(1.0f);
//	mat4 view = lookAt(vec3(0, 0, 8),
//		vec3(0, 0, -1), vec3(0, 1, 0));//center, lookat, up
//
//	mat4 projection = perspective(radians(60.0f),
//		(float)4.0 / (float)3.0, 0.1f, 100.0f);
//
//	model = translate(model, vec3(0.0f, 0.0f, -1.0f));
//	//model = glm::rotate(model, 0.5236f, vec3(0, 1, 0));
//
//	//this helps translate the light source
//	mat4 model2 = translate(mat4(1.0f), vec3(10.0f, 4.0f, -1.0f));
//
//	
//	
//	
//
//	//Shader s("vertex.shader","fragment.shader");
//	Shader s("Spotlight_VS.shader", "Spotlight_FS.shader");
//	//Shader u("lightVertex.shader", "lightFragment.shader");
//	Shader u("Spotlight_VS.shader", "Spotlight_FS.shader");
//
//	
//
//	/*vec3 lightpos = vec3(-6.5, -10, -1);
//	string vertexsource = ParseShader("vertex.shader");
//	string fragmentsource = ParseShader("fragment.shader");
//
//	unsigned int program = CreateShader(vertexsource, fragmentsource);
//
//	string lightvertsource = ParseShader("lightVertex.shader");
//	string lightfragsource = ParseShader("lightFragment.shader");
//
//	unsigned int lightprogram = CreateShader(lightvertsource, lightfragsource);*/
//	/*unsigned int viewprojspace = glgetuniformlocation(program, "mvp");
//	gluniformmatrix4fv(viewprojspace, 1, gl_false, value_ptr(mvp));*/
//
//	//unsigned int worldspace = glGetUniformLocation(program, "model");
//	s.use();
//	s.setInt("material.diffuse", 0);
//	s.setInt("material.specular", 1);
//
//	u.use();
//	u.setInt("material.diffuse", 0);
//	u.setInt("material.specular", 1);
//	
//	/* loop until the user closes the window */
//	while (!window.windowShouldClose())
//	{
//		
//		
//		processInput(window.getWindow());
//
//		
//		glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
//
//		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//		
//		// render here 
//		//glclearcolor(0.0f, 0.0f, 0.0f, 0.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		//float distance = -0.1;
//		mat4 lightSpan = glm::rotate(model,glm::radians(lightAngle),vec3(0,1,0));
//		
//		
//
//		mat4 mvp = projection * view * model;
//		mat4 mvplight = projection * view * model * model2;
//		mat4 mv = view * model;
//		
//		//glUseProgram(program);
//		s.use();
//		
//		/*glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(mvp));
//		glUniformMatrix4fv(worldspace, 1, GL_FALSE, value_ptr(model));
//		glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, value_ptr(cameraPos));*/
//		
//		
//		s.setFloat("mvp", 1, mvp);
//		s.setFloat("model",1,model);
//		s.setFloat("view", 1, view);
//		s.setFloat("viewPos", 1, cameraPos);
//		s.setFloat("lightSpan", 1, lightSpan);
//		s.setFloat("lightTranslate", 1, lightTranslate);
//
//		
//		
//		//s.setVector("material.specular", 1, 1.0f, 1.0f, 1.0f);
//		s.setFloat("material.shininess", 128);
//
//		s.setVector("light.position", 1, 2, 5, 1);
//		s.setFloat("light.ambientStrength", 0.1);
//		s.setFloat("light.diffuseStrength", 0.5);
//		s.setFloat("light.specularStrength", 0.8);
//		s.setFloat("light.constant", 1);
//		s.setFloat("light.linear", 0.0);
//		s.setFloat("light.quadratic", 0.032);
//		
//
//		s.setVector("light.position", 1, cameraPos.x, cameraPos.y, cameraPos.z);
//		s.setVector("light.direction", 1, cameraFront.x, cameraFront.y, cameraFront.z);
//		
//		s.setFloat("light.clipAngle", cos(glm::radians(12.5)));
//		s.setFloat("light.outerClipAngle", cos(glm::radians(17.5)));
//
//		/*
//		s.setVector("material.ambient", 1, vec3(1.0f, 0.0f, 0.0f));
//		s.setVector("material.diffuse", 1, vec3(0.0f, 1.0f, 0.0f));
//		s.setVector("material.specular", 1, vec3(1.0f, 1.0f, 1.0f));
//		*/
//		//s.setVector("material.shininess")
//
//		//gluniformmatrix4fv(glgetuniformlocation(program, "mv"), 1, gl_false, value_ptr(mv));
//		// where worldspace = glgetuniformlocation(program, "model");
//		
//		glActiveTexture(GL_TEXTURE0);
//		Cube.bindDiffuseTexture();
//		glActiveTexture(GL_TEXTURE1);
//		Cube.bindSpecularTexture();
//
//		glActiveTexture(GL_TEXTURE3);
//		Floor.bindDiffuseTexture();
//		glActiveTexture(GL_TEXTURE4);
//		Floor.bindSpecularTexture();
//
//		glActiveTexture(GL_TEXTURE5);
//		LightCube.bindDiffuseTexture();
//		glActiveTexture(GL_TEXTURE6);
//		LightCube.bindSpecularTexture();
//		//
//		//container.bindTexture2D();
//		//glBindVertexArray(vao);
//		//VertexArray v = foo.getVAO();
//		//(foo.getVAO()).bindVAO(0); // here, it appears that I am activating a method of a class upon something taht is not a class as it clearly returns unsigned int 
//		
//		Cube.getVAO()->bindVAO(0);
//
//		//sum of faces * 3
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//
//		Floor.getVAO()->bindVAO(0);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		
//		u.use();
//
//		u.setFloat("mvp", 1, mvp);
//		u.setFloat("model", 1, model);
//		u.setFloat("view", 1, view);
//		u.setFloat("viewPos", 1, cameraPos);
//		u.setFloat("lightSpan", 1, lightSpan);
//		u.setFloat("lightTranslate", 1, lightTranslate);
//
//
//
//		//u.setVector("material.specular", 1, 1.0f, 1.0f, 1.0f);
//		u.setFloat("material.shininess", 128);
//
//		u.setVector("light.position", 1, 2, 5, 1);
//		u.setFloat("light.ambientStrength", 0.1);
//		u.setFloat("light.diffuseStrength", 0.5);
//		u.setFloat("light.specularStrength", 0.8);
//		u.setFloat("light.constant", 1);
//		u.setFloat("light.linear", 0.0);
//		u.setFloat("light.quadratic", 0.032);
//
//
//		u.setVector("light.position", 1, cameraPos.x, cameraPos.y, cameraPos.z);
//		u.setVector("light.direction", 1, cameraFront.x, cameraFront.y, cameraFront.z);
//
//		u.setFloat("light.clipAngle", cos(glm::radians(12.5)));
//		u.setFloat("light.outerClipAngle", cos(glm::radians(17.5)));
//		//glUseProgram(lightprogram);
//
//		//gluniformmatrix4fv(glgetuniformlocation(lightprogram, "mvp"), 1, gl_false, value_ptr(mvp));
//		//gluniformmatrix4fv(glgetuniformlocation(program, "mv"), 1, gl_false, value_ptr(mv));
//		//u.setFloat("mvplight",1,mvplight);
//		//glUniform3fv(glGetUniformLocation(lightprogram, "mvplight"), 1, value_ptr(mvplight));
//
//		LightCube.getVAO()->bindVAO(0);
//		//glBindVertexArray(lightvao);
//
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//		
//		window.swapAndPoll();
//	}
//
//
//	s.Delete();
//	u.Delete();
//
//	glfwTerminate();
//	return 0;
//}
