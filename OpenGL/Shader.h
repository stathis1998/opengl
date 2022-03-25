#pragma once

class Shader {
private:
	unsigned int program;

  unsigned int compileShader(const char* shaderCode, unsigned int type);
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

  void use();
};

