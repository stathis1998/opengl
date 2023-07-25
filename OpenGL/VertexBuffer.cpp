#include "VertexBuffer.h"

#include <GL/glew.h>

VertexBuffer::VertexBuffer(const void* data, const unsigned int size) {
	glGenBuffers(1, &this->VBO);
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &this->VBO);
}

void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
}