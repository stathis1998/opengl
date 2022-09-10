#pragma once

class Shader {
private:
	unsigned int program;
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	unsigned int compile(const char* code, unsigned int type);
	void use();
};

