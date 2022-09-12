#include "ElementBuffer.h"

#include <GL/glew.h>

ElementBuffer::ElementBuffer(const unsigned int* data, const unsigned int count): count(count) {
	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer() {
	glDeleteBuffers(1, &this->EBO);
}

void ElementBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
}

void ElementBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}