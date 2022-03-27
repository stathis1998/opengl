#include "Shader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	this->program = glCreateProgram();

	std::stringstream vertexStream, fragmentStream;
	std::ifstream vertexFile, fragmentFile;
	std::string vertexCode, fragmentCode;

	// Set exceptions for the files
	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// Read the shaders from the files
	try {
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);

		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();
	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return;
	}

	// Compile the shaders
	unsigned int vertexShader = this->compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
	unsigned int fragmentShader = this->compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

	// Attach the shaders to the program
	glAttachShader(this->program, vertexShader);
	glAttachShader(this->program, fragmentShader);

	// Link the program
	glLinkProgram(this->program);

	// Check for errors
	int success;
	glGetProgramiv(this->program, GL_LINK_STATUS, &success);
	if (!success) {
		// Get info log length
		int infoLogLength;
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(this->program, infoLogLength, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		delete[] infoLog;
		return;
	}

	// Validate the program
	glValidateProgram(this->program);

	// Check for errors
	glGetProgramiv(this->program, GL_VALIDATE_STATUS, &success);
	if (!success) {
		// Get info log length
		int infoLogLength;
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(this->program, infoLogLength, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::VALIDATION_FAILED\n" << infoLog << std::endl;
		delete[] infoLog;
		return;
	}

	// Delete the shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Set the program as active
	this->use();
}

Shader::~Shader() {
	glDeleteProgram(this->program);
}

unsigned int Shader::compileShader(const char* shaderCode, unsigned int type) {
	// Create the shader
	unsigned int shader = glCreateShader(type);

	// Compile the shader
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);

	// Check for errors
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		// Get ingo log length
		int infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		delete[] infoLog;

		return -1;
	}

	return shader;
}

void Shader::use() {
	glUseProgram(this->program);
}

void Shader::setMat4(const char* name, glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(value));
}