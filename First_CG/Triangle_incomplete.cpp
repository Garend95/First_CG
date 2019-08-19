#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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
		cout << message << endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

// takes the shader codes as a string parameters 
static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader)
{
	GLuint program =  glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER  , vertexShader  );
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

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);*/

	glClearDepth(-1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);

	float coordinates[] = {
		0,0,0,  255,0,0, 0,0,1,
		4,4,0, 	255,0,0, 0,0,1,
		0,4,0, 	255,0,0, 0,0,1,
		0,0,0, 	255,0,0, 0,0,1,
		4,0,0, 	255,0,0, 0,0,1,
		4,4,0, 	255,0,0, 0,0,1,
		4,0,0, 	255,0,0, 1,0,0,
		4,4,-4,	 255,0,0, 1,0,0,
		4,4,0, 	255,0,0, 1,0,0,
		4,0,0, 	255,0,0, 1,0,0,
		4,0,-4,	 255,0,0, 1,0,0,
		4,4,-4,	 255,0,0, 1,0,0,
		0,0,-4,	 255,0,0, 0,0,-1,
		4,4,-4,	 255,0,0, 0,0,-1,
		4,0,-4,	 255,0,0, 0,0,-1,
		0,0,-4,	 255,0,0, 0,0,-1,
		0,4,-4,	 255,0,0, 0,0,-1,
		4,4,-4,	 255,0,0, 0,0,-1,
		0,0,0, 	255,0,0, -1,0,0,
		0,4,-4,	 255,0,0, -1,0,0,
		0,0,-4,	 255,0,0, -1,0,0,
		0,0,0, 	255,0,0, -1,0,0,
		0,4,0, 	255,0,0,  -1,0,0,
		0,4,-4,	 255,0,0, -1,0,0,
		4,4,0, 	255,0,0,   0,1,0,
		4,4,-4,	 255,0,0,  0,1,0,
		0,4,-4,	 255,0,0,  0,1,0,
		4,4,0, 	255,0,0,   0,1,0,
		0,4,-4,	 255,0,0,  0,1,0,
		0,4,0,	255,0,0	   0,1,0,
		4,0,0, 	255,0,0,   0,-1,0,
		4,0,-4,	 255,0,0,  0,-1,0,
		0,0,-4,	 255,0,0,  0,-1,0,
		4,0,0, 	255,0,0,   0,-1,0,
		0,0,-4,	 255,0,0,  0,-1,0,
		0,0,0,	255,0,0	   0,-1,0
	
	};

	float lightCoordinates[] = {
		7,0,0, 255,255,255, 0,0,1,
		7,1,0, 255,255,255,	0,0,1,
		6,1,0, 255,255,255,	0,0,1,
		7,0,0, 255,255,255,	0,0,1,
		6,1,0, 255,255,255,	0,0,1,
		6,0,0, 255,255,255	0,0,1
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

	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightCoordinates), lightCoordinates, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 9, (GLvoid*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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
	Model = glm::rotate(Model, 0.5236f, vec3(0,1,0));
	

	string vertexSource =   ParseShader("vertex.shader");
	string fragmentSource = ParseShader("fragment.shader");

	unsigned int program = CreateShader(vertexSource, fragmentSource);

	string lightVertSource = ParseShader("lightVertex.shader");
	string lightFragSource = ParseShader("lightFragment.shader");
	
	unsigned int lightProgram = CreateShader(lightVertSource, lightFragSource);
	/*unsigned int viewProjSpace = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(viewProjSpace, 1, GL_FALSE, value_ptr(mvp));*/

	unsigned int worldSpace = glGetUniformLocation(program, "Model")
	//glUseProgram(program);

	//unsigned int viewProjSpace = glGetUniformLocation(program, "mvp");


	float angle = 0;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Render here 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//float distance = -0.1;
		glUseProgram(program);
		//mat4 Model = glm::rotate(Model, 57.29578f, vec3(0,1,0));
		mat4 mvp = Projection * View * Model;
		mat4 mv = View * Model;
		glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(mvp));
		glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_FALSE, value_ptr(mv));
		glUniformMatrix4fv(worldSpace, 1, GL_FALSE, value_ptr(Model)); 

		glBindVertexArray(VAO);

		//sum of faces * 3
		 glDrawArrays(GL_TRIANGLES,  0, 36);
		


		 glUseProgram(lightProgram);

		 glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(mvp));
		 glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_FALSE, value_ptr(mv));
		 glUniformMatrix4fv(worldSpace, 1, GL_FALSE, value_ptr(Model));

		 glBindVertexArray(lightVAO);

		 glDrawArrays(GL_TRIANGLES, 0, 6);
		 //Swap front and back buffers 
		
		 glfwSwapBuffers(window);
		// Poll for and process events 
		glfwPollEvents();
	}
	

	glDeleteProgram(program);

	glfwTerminate();
	return 0;
}