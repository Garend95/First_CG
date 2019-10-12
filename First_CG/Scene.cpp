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
#include "Texture.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Mesh2.h"

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

// takes the shader codes as a string parameters 
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

vec3 cameraPos = vec3(0.0f, 0.0f, 8.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

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
}





int main(void)
{
	GLFWwindow* window;

	/* initialize the library */
	if (!glfwInit())
	{
		return -1;
	};
	
	/* create a windowed mode window and its opengl context */
	window = glfwCreateWindow(1280, 800, "hello world", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwSwapInterval(1);
	/* make the window's context current */
	glfwMakeContextCurrent(window);

	// call glewinit after creating the context...
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* problem: glewinit failed, something is seriously wrong. */
		fprintf(stderr, "error: %s\n", glewGetErrorString(err));
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	/*glcleardepth(-1);
	glenable(gl_depth_test);
	gldepthfunc(gl_less);*/
	
	
	vector<float> coordinates = {
		0,0,0,  255,100,0, 0,0,1,    
		4,4,0, 	255,100,0, 0,0,1,    
		0,4,0, 	255,100,0, 0,0,1,    
		0,0,0, 	255,100,0, 0,0,1,    
		4,0,0, 	255,100,0, 0,0,1,    
		4,4,0, 	255,100,0, 0,0,1,    
		4,0,0, 	255,100,0, 1,0,0,    
		4,4,-4,	 255,100,0, 1,0,0,   
		4,4,0, 	255,100,0, 1,0,0,    
		4,0,0, 	255,100,0, 1,0,0,    
		4,0,-4,	 255,100,0, 1,0,0,   
		4,4,-4,	 255,100,0, 1,0,0,   
		0,0,-4,	 255,100,0, 0,0,-1,  
		4,4,-4,	 255,100,0, 0,0,-1,  
		4,0,-4,	 255,100,0, 0,0,-1,  
		0,0,-4,	 255,100,0, 0,0,-1,  
		0,4,-4,	 255,100,0, 0,0,-1,  
		4,4,-4,	 255,100,0, 0,0,-1,  
		0,0,0, 	255,100,0, -1,0,0,   
		0,4,-4,	 255,100,0, -1,0,0,  
		0,0,-4,	 255,100,0, -1,0,0,  
		0,0,0, 	255,100,0, -1,0,0,   
		0,4,0, 	255,100,0,  -1,0,0,  
		0,4,-4,	 255,100,0, -1,0,0,  
		4,4,0, 	255,100,0,   0,1,0,  
		4,4,-4,	 255,100,0,  0,1,0,  
		0,4,-4,	 255,100,0,  0,1,0,  
		4,4,0, 	255,100,0,   0,1,0,  
		0,4,-4,	 255,100,0,  0,1,0,  
		0,4,0,	255,100,0,   0,1,0,  
		0,0,-4, 255,100,0,   0,-1,0, 
		4,0,-4,	 255,100,0,  0,-1,0, 
		4,0,0,	 255,100,0,  0,-1,0, 
		0,0,0, 	255,100,0,   0,-1,0, 
		0,0,-4,	 255,100,0,  0,-1,0, 
		4,0,0,	255,100,0,   0,-1,0

	};


	/*0, 0,
		1, 1,
		0, 1,
		0, 0,
		1, 0,
		1, 1,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0*/

	vector<float> lightcoordinates = {
		7,0,0,  255,255,255, 0,0,1, 
		7,1,0,  255,255,255, 0,0,1,
		6,1,0,  255,255,255, 0,0,1,
		7,0,0,  255,255,255, 0,0,1,
		6,1,0,  255,255,255, 0,0,1,
		6,0,0,  255,255,255, 0,0,1,
		7,0,0,  255,255,255, 1,0,0,
		7,1,-1,	255,255,255,  1,0,0,
		7,1,0,	255,255,255, 1,0,0,
		7,0,0,	255,255,255, 1,0,0,
		7,0,-1,	255,255,255,  1,0,0,
		7,1,-1,	255,255,255,  1,0,0,
		6,0,-1, 255,255,255,  0,0,-1,
		7,1,-1,	255,255,255,  0,0,-1,
		7,0,-1,	255,255,255,  0,0,-1,
		6,0,-1,	255,255,255,  0,0,-1,
		6,1,-1,	255,255,255,  0,0,-1,
		7,1,-1,	255,255,255,  0,0,-1,
		6,0,0,  255,255,255, -1,0,0,
		6,1,-1,	255,255,255,  -1,0,0,
		6,0,-1,	255,255,255,  -1,0,0,
		6,0,0,	255,255,255, -1,0,0,
		6,1,0,	255,255,255,  -1,0,0,
		6,1,-1,	255,255,255,  -1,0,0,
		7,1,0,  255,255,255,   0,1,0,
		7,1,-1,	255,255,255,   0,1,0,
		6,1,-1,	255,255,255,   0,1,0,
		7,1,0,	255,255,255,   0,1,0,
		6,1,-1,	255,255,255,   0,1,0,
		6,1,0,	255,255,255,   0,1,0,
		6,0,-1,  255,255,255,   0,-1,0,// 7,0,0,
		7,0,-1,	255,255,255,   0,-1,0,// 7,0,-1
		7,0,0,	255,255,255,   0,-1,0,// 6,0,-1
		6,0,0,	255,255,255,   0,-1,0,// 7,0,0,
		6,0,-1,	255,255,255,   0,-1,0,// 6,0,-1
		7,0,0,	255,255,255,   0,-1,0//	 6,0,0,
	};

	/*Buffer cube;
	cube.setSize(1);
	cube.generateBuffers(1);
	cube.bindBuffer(0);
	cube.provideBufferData(coordinates, sizeof(coordinates),GL_STATIC_DRAW);*/
	Mesh2 Cube(coordinates);

	Cube.assignPointersInVAO(true, true, false);
	Mesh2 LightCube(lightcoordinates);
	LightCube.assignPointersInVAO(true, true, false);
	//unsigned int vao;
	//unsigned int buffer[1], vao; //buffer for coordinates
	/*glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);*/
	
	/*VertexArray m;
	m.setVASize(1);
	m.generateVAO(1, 0);
	m.bindVAO(0);
	m.assignPointers3D(true, true, false);*/

	/*glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//our stride is 6 to accomodate for both the coordinates and colors of the cube
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);*/

	/*glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(float) * 11, (GLvoid*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);*/
	
	//for the light source
	/*unsigned int lightvao;
	glGenVertexArrays(1, &lightvao);
	glBindVertexArray(lightvao);

	//do i need to bind the second buffer at all? can i make do by the first one only??
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightcoordinates), lightcoordinates, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	*/

	/*Texture container;
	container.generateAndBindTexture2D(GL_REPEAT);
	container.loadTexture("C://Users//Garen//source//repos//First_CG//Dependencies//Misc//container.png");
	container.freeImage();*/

	
	/*glvertexattribpointer(2, 3, gl_float, gl_true, sizeof(float) * 9, (glvoid*)(6 * sizeof(float)));
	glenablevertexattribarray(2);*/

	/*unsigned int lightvao;
	glgenvertexarrays(1, &lightvao);
	glbindvertexarray(lightvao);

	glbindbuffer(gl_array_buffer, lightvao);

	glvertexattribpointer(1, 3, gl_float, gl_true, sizeof(float) * 3, (glvoid*)vertex_position_offset);
	glenablevertexattribarray(1);*/

	mat4 model = mat4(1.0f);
	mat4 view = lookAt(vec3(0, 0, 8),
		vec3(0, 0, -1), vec3(0, 1, 0));//center, lookat, up

	mat4 projection = perspective(radians(60.0f),
		(float)4.0 / (float)3.0, 0.1f, 100.0f);

	model = translate(model, vec3(0.0f, 0.0f, -1.0f));
	//model = glm::rotate(model, 0.5236f, vec3(0, 1, 0));

	//this helps translate the light source
	mat4 model2 = translate(mat4(1.0f), vec3(10.0f, 4.0f, -1.0f));

	vec3 lightpos = vec3(-6.5, -10, -1);
	string vertexsource = ParseShader("vertex.shader");
	string fragmentsource = ParseShader("fragment.shader");

	unsigned int program = CreateShader(vertexsource, fragmentsource);

	string lightvertsource = ParseShader("lightVertex.shader");
	string lightfragsource = ParseShader("lightFragment.shader");

	unsigned int lightprogram = CreateShader(lightvertsource, lightfragsource);
	/*unsigned int viewprojspace = glgetuniformlocation(program, "mvp");
	gluniformmatrix4fv(viewprojspace, 1, gl_false, value_ptr(mvp));*/

	unsigned int worldspace = glGetUniformLocation(program, "model");
		
	float angle = 0;
	/* loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		
		
		processInput(window);

		
		//glfwSetCursorPosCallback(window, mouse_callback);

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		// render here 
		//glclearcolor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//float distance = -0.1;
		mat4 mvp = projection * view * model;
		mat4 mvplight = projection * view * model * model2;
		mat4 mv = view * model;
		
		glUseProgram(program);
		
		glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(mvp));
		//gluniformmatrix4fv(glgetuniformlocation(program, "mv"), 1, gl_false, value_ptr(mv));
		glUniformMatrix4fv(worldspace, 1, GL_FALSE, value_ptr(model));
		glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, value_ptr(cameraPos));
		// where worldspace = glgetuniformlocation(program, "model");
		
		 
		//container.bindTexture2D();
		//glBindVertexArray(vao);
		//VertexArray v = foo.getVAO();
		//(foo.getVAO()).bindVAO(0); // here, it appears that I am activating a method of a class upon something taht is not a class as it clearly returns unsigned int 
		Cube.getVAO()->bindVAO(0);

		//sum of faces * 3
		glDrawArrays(GL_TRIANGLES, 0, 36);

		

		glUseProgram(lightprogram);

		//gluniformmatrix4fv(glgetuniformlocation(lightprogram, "mvp"), 1, gl_false, value_ptr(mvp));
		//gluniformmatrix4fv(glgetuniformlocation(program, "mv"), 1, gl_false, value_ptr(mv));
		glUniform3fv(glGetUniformLocation(lightprogram, "mvplight"), 1, value_ptr(mvplight));

		LightCube.getVAO()->bindVAO(0);
		//glBindVertexArray(lightvao);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//swap front and back buffers 
		glfwSwapBuffers(window);
		// poll for and process events 
		glfwPollEvents();
	}


	glDeleteProgram(program);

	glfwTerminate();
	return 0;
}
