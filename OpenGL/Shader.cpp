#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath): program(0) {
	// Reading the shader files
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile, fShaderFile;
	std::stringstream vShaderStream, fShaderStream;

	// File exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// Opening the files
	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	} catch (std::ifstream::failure e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// Compiling the shaders
	unsigned int vertex, fragment;
	vertex = this->compile(vShaderCode, GL_VERTEX_SHADER);
	fragment = this->compile(fShaderCode, GL_FRAGMENT_SHADER);

	// Linking the shaders
	this->program = glCreateProgram();
	// Attaching shaders to program
	glAttachShader(this->program, vertex);
	glAttachShader(this->program, fragment);
	// Linking program
	glLinkProgram(this->program);
	// Checking for errors
	int success;
	char* infoLog;
	glGetProgramiv(this->program, GL_LINK_STATUS, &success);
	if (!success) {
		int logLength;
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &logLength);
		infoLog = new char[logLength];
		glGetProgramInfoLog(this->program, logLength, nullptr, infoLog);
		std::cerr << "ERROR::PROGRAM::LINKING_FAILED" << std::endl;
		std::cerr << infoLog << std::endl;
		delete[] infoLog;
	}

	// Validating the program
	glValidateProgram(this->program);
	glGetProgramiv(this->program, GL_VALIDATE_STATUS, &success);
	if (!success) {
		int logLength;
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &logLength);
		infoLog = new char[logLength];
		glGetProgramInfoLog(this->program, logLength, nullptr, infoLog);
		std::cerr << "ERROR::PROGRAM::VALIDATION_FAILED" << std::endl;
		std::cerr << infoLog << std::endl;
		delete[] infoLog;
	}

	// Deleting shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader() {
	// Deleting program
	glDeleteProgram(this->program);
}

unsigned int Shader::compile(const char* code, unsigned int type) {
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &code, nullptr);
	glCompileShader(shader);

	// Checking for errors
	int success;
	char* infoLog;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		int logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		infoLog = new char[logLength];
		glGetShaderInfoLog(shader, logLength, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED" << std::endl;
		std::cerr << infoLog << std::endl;
		delete[] infoLog;
	}

	return shader;
}

void Shader::use() {
	glUseProgram(this->program);
}

void Shader::setBool(const std::string& name, bool v0) const {
	glUniform1i(glGetUniformLocation(this->program, name.c_str()), (int) v0);
}

void Shader::setBool(const std::string& name, bool v0, bool v1) const {
	glUniform2i(glGetUniformLocation(this->program, name.c_str()), (int) v0, (int) v1);
}

void Shader::setBool(const std::string& name, bool v0, bool v1, bool v2) const {
	glUniform3i(glGetUniformLocation(this->program, name.c_str()), (int) v0, (int) v1, (int) v2);
}

void Shader::setBool(const std::string& name, bool v0, bool v1, bool v2, bool v3) const {
	glUniform4i(glGetUniformLocation(this->program, name.c_str()), (int) v0, (int) v1, (int) v2, (int) v3);
}

void Shader::setInt(const std::string& name, int v0) const {
	glUniform1i(glGetUniformLocation(this->program, name.c_str()), v0);
}

void Shader::setInt(const std::string& name, int v0, int v1) const {
	glUniform2i(glGetUniformLocation(this->program, name.c_str()), v0, v1);
}

void Shader::setInt(const std::string& name, int v0, int v1, int v2) const {
	glUniform3i(glGetUniformLocation(this->program, name.c_str()), v0, v1, v2);
}

void Shader::setInt(const std::string& name, int v0, int v1, int v2, int v3) const {
	glUniform4i(glGetUniformLocation(this->program, name.c_str()), v0, v1, v2, v3);
}

void Shader::setFloat(const std::string& name, float v0) const {
	glUniform1f(glGetUniformLocation(this->program, name.c_str()), v0);
}

void Shader::setFloat(const std::string& name, float v0, float v1) const {
	glUniform2f(glGetUniformLocation(this->program, name.c_str()), v0, v1);
}

void Shader::setFloat(const std::string& name, float v0, float v1, float v2) const {
	glUniform3f(glGetUniformLocation(this->program, name.c_str()), v0, v1, v2);
}

void Shader::setFloat(const std::string& name, float v0, float v1, float v2, float v3) const {
	glUniform4f(glGetUniformLocation(this->program, name.c_str()), v0, v1, v2, v3);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}