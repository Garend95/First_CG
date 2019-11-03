#include "Shader.h"

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    this->ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}

string Shader::ParseShader(string filepath) {
	ifstream stream(filepath);
	string line;
	stringstream stringStream;

	while (getline(stream, line))
	{
		stringStream << line << '\n';
	}

	return stringStream.str();
}

int Shader::createShader(const string& vertexShader, const string& fragmentShader) {

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

int Shader::CompileShader(unsigned int type, const string& source) {
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
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
};

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value,  glm::mat4 matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), value, GL_FALSE, value_ptr(matrix));
   
    //glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

//overloaded with a vector instead of a matrix
void Shader::setFloat(const std::string& name, float value, glm::vec3 vector) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), value, value_ptr(vector));
   
    //glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float index) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), index);
}

/*
void Shader::setVector(const std::string& name, float count, glm::vec3 vector ) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), count, value_ptr(glm::vec3(vector.x, vector.y, vector.z));
}
*/


void Shader::setVector(const std::string& name, float count, float Xvalue, float Yvlaue, float Zvalue) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), count, value_ptr(glm::vec3(Xvalue, Yvlaue, Zvalue)));
}
/////PRIVATE

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}