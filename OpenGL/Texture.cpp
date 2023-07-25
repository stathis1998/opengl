#include <GL/glew.h>
#include <iostream>

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(unsigned int textureUnit) : textureID(0), width(0), height(0), nrChannels(0), textureUnit(textureUnit) {
	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &this->textureID);
	this->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture() {
	glDeleteTextures(1, &this->textureID);
}

void Texture::load(const char* fileName) {
	unsigned char* data = stbi_load(fileName, &this->width, &this->height, &this->nrChannels, 0);

	unsigned int format = this->nrChannels == 3 ? GL_RGB : GL_RGBA;

	if (data) {
		this->bind();
		glTexImage2D(GL_TEXTURE_2D, 0, format, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cerr << "ERROR::STB_IMAGE::FAILED_TO_LOAD_IMAGE" << std::endl;
	}

	std::cout << this->width << " " << this->height << " " << this->nrChannels << std::endl;

	stbi_image_free(data);
}

void Texture::bind() const {
	glActiveTexture(this->textureUnit);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
}