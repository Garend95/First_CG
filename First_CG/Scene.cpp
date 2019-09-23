#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Exporter.hpp>

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

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 800, "Hello World", NULL, NULL);
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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	/*glClearDepth(-1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);*/
	
	float coordinates[] = {
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
		0,0,-4, 255,100,0,   0,-1,0,// 4,0,0, 	
		4,0,-4,	 255,100,0,  0,-1,0,// 4,0,-4,	
		4,0,0,	 255,100,0,  0,-1,0,// 0,0,-4,	
		0,0,0, 	255,100,0,   0,-1,0,// 4,0,0, 	
		0,0,-4,	 255,100,0,  0,-1,0,// 0,0,-4,	
		4,0,0,	255,100,0,   0,-1,0//	 0,0,0,	

	};

	float lightCoordinates[] = {
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

	unsigned int buffer[2], VAO; //buffer for coordinates
	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//our stride is 6 to accomodate for both the coordinates and colors of the cube
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//for the light source
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	//do I need to bind the second buffer at all? Can I make do by the first one only??
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightCoordinates), lightCoordinates, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/*glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);*/

	/*unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, lightVAO);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 3, (GLvoid*)vertex_position_offset);
	glEnableVertexAttribArray(1);*/

	mat4 Model = mat4(1.0f);
	mat4 View = lookAt(vec3(0, 0, 8),
		vec3(0, 0, -1), vec3(0, 1, 0));//center, lookat, up

	mat4 Projection = perspective(radians(60.0f),
		(float)4.0 / (float)3.0, 0.1f, 100.0f);

	Model = translate(Model, vec3(0.0f, 0.0f, -1.0f));
	//Model = glm::rotate(Model, 0.5236f, vec3(0, 1, 0));

	//this helps translate the light source
	mat4 Model2 = translate(mat4(1.0f), vec3(10.0f, 4.0f, -1.0f));

	vec3 lightPos = vec3(-6.5, -10, -1);
	string vertexSource = ParseShader("vertex.shader");
	string fragmentSource = ParseShader("fragment.shader");

	unsigned int program = CreateShader(vertexSource, fragmentSource);

	string lightVertSource = ParseShader("lightVertex.shader");
	string lightFragSource = ParseShader("lightFragment.shader");

	unsigned int lightProgram = CreateShader(lightVertSource, lightFragSource);
	/*unsigned int viewProjSpace = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(viewProjSpace, 1, GL_FALSE, value_ptr(mvp));*/

	unsigned int worldSpace = glGetUniformLocation(program, "Model");
		//glUseProgram(program);

		//unsigned int viewProjSpace = glGetUniformLocation(program, "mvp");

	/* // Camera attributes for rotating around the 
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	*/

	float angle = 0;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		
		
		processInput(window);

		
		glfwSetCursorPosCallback(window, mouse_callback);

		View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		// Render here 
		//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//float distance = -0.1;
		mat4 mvp = Projection * View * Model;
		mat4 mvpLight = Projection * View * Model * Model2;
		mat4 mv = View * Model;
		
		glUseProgram(program);
		
		glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(mvp));
		//glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_FALSE, value_ptr(mv));
		glUniformMatrix4fv(worldSpace, 1, GL_FALSE, value_ptr(Model));
		glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, value_ptr(cameraPos));
		// where worldSpace = glGetUniformLocation(program, "Model");
		
		

		glBindVertexArray(VAO);

		//sum of faces * 3
		glDrawArrays(GL_TRIANGLES, 0, 36);

		

		glUseProgram(lightProgram);

		//glUniformMatrix4fv(glGetUniformLocation(lightProgram, "mvp"), 1, GL_FALSE, value_ptr(mvp));
		//glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_FALSE, value_ptr(mv));
		glUniformMatrix4fv(glGetUniformLocation(lightProgram, "mvpLight"), 1, GL_FALSE, value_ptr(mvpLight));

		glBindVertexArray(lightVAO);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//Swap front and back buffers 
		glfwSwapBuffers(window);
		// Poll for and process events 
		glfwPollEvents();
	}


	glDeleteProgram(program);

	glfwTerminate();
	return 0;
}