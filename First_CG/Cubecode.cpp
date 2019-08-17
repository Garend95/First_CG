#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
	glFrontFace(GL_CW);
	
	float vert2[] = {
		0,0,
		4,0,
		0,4,/*,
		0,0,-4,
		0,4,0,
		4,4,0,
		4,4,-4,
		0,4,-4*/
	};
	
	float ind2[] = {
		0
	};

	vector<float> vertices;

	vector<int> indices;
	


	vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	mat4 trans = mat4(1.0f);
	trans = translate(trans, vec3(0, 0, -1));

	mat4 Model = mat4(1.0f);
	mat4 View = lookAt(vec3(0, 0, 8),
		vec3(0, 0, -1), vec3(0, 1, 0));//center, lookat, up


	mat4 Projection = perspective(radians(45.0f),
		(float)4.0 / (float)3.0, 0.1f, 100.0f);

	mat4 mvp = Projection * View * Model * trans;

	//Let's multiply each of the elements of the verticies represented in a vector

	/*
	for (int i = 0; i < vertices.size(); i += 3) {
		vec4 vert(vertices[i], vertices[i + 1], vertices[i + 2], 1);

		vec4 result = mvp * vert;

		vertices.at(i) = result.x;
		vertices.at(i + 1) = result.y;
		vertices.at(i + 2) = result.z;
	}
	*/


	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert2) * sizeof(float), &vert2[0], GL_STATIC_DRAW);

	unsigned int indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind2) * sizeof(int), &ind2[0], GL_STATIC_DRAW);

	GLintptr vertex_position_offset = 0 * sizeof(float);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid*)vertex_position_offset);
	glEnableVertexAttribArray(0);


	string vertexSource = ParseShader("vertex.shader");
	string fragmentSource = ParseShader("fragment.shader");

	unsigned int program = CreateShader(vertexSource, fragmentSource);
	glUseProgram(program);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Render here 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		glDrawArrays(GL_TRIANGLES, GL_FLOAT, 0);
	
		//Swap front and back buffers 
		glfwSwapBuffers(window);

		// Poll for and process events 
		glfwPollEvents();
	}


	glDeleteProgram(program);

	glfwTerminate();
	return 0;
}