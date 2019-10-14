#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;


class Shader
{

private:
	// the program ID
	unsigned int ID;

public:
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	~Shader();

	string ParseShader(string filepath);
	
	int createShader(const string& vertexShader, const string& fragmentShader);

	int CompileShader(unsigned int type, const string& source);
	
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string& name, bool value) const;

	void setInt(const std::string& name, int value) const;
	
	void setFloat(const std::string& name, float value, glm::mat4 matrix) const;

	void setFloat(const std::string& name, float value, glm::vec3 vector) const;


	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type);
	
};



#endif