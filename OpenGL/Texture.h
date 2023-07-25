#pragma once

class Texture {
private:
	unsigned int textureID;
	int width, height, nrChannels;
	unsigned int textureUnit;
public:
	Texture(unsigned int textureUnit);
	~Texture();

	void load(const char* fileName, unsigned int internalFormat, unsigned int format);
	void bind();
	void unbind();
};

