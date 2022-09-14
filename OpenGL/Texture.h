#pragma once

#include <GL/glew.h>

class Texture {
private:
	unsigned int texture;
	int width, height, nrChannels;
public:
	Texture(const char* path);
	~Texture();

	void bind();
	void unbind();
};