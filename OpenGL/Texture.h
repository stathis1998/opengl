#pragma once

class Texture {
private:
	unsigned int textureID;
	int width, height, nrChannels;
	unsigned int textureUnit;
public:
	Texture(unsigned int textureUnit);
	~Texture();

	void load(const char* fileName);
	void bind() const;
};

