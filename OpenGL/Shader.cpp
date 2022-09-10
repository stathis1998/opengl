#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
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