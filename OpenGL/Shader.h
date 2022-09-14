#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
	unsigned int program;
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	unsigned int compile(const char* code, unsigned int type);
	void use();

	inline unsigned int getProgram() { return this->program; }

	void setBool(const std::string& name, bool v0) const;
	void setBool(const std::string& name, bool v0, bool v1) const;
	void setBool(const std::string& name, bool v0, bool v1, bool v2) const;
	void setBool(const std::string& name, bool v0, bool v1, bool v2, bool v3) const;

	void setInt(const std::string& name, int v0) const;
	void setInt(const std::string& name, int v0, int v1) const;
	void setInt(const std::string& name, int v0, int v1, int v2) const;
	void setInt(const std::string& name, int v0, int v1, int v2, int v3) const;

	void setFloat(const std::string& name, float v0) const;
	void setFloat(const std::string& name, float v0, float v1) const;
	void setFloat(const std::string& name, float v0, float v1, float v2) const;
	void setFloat(const std::string& name, float v0, float v1, float v2, float v3) const;

	void setMat4(const std::string& name, const glm::mat4& mat) const;
};

