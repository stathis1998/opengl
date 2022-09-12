#include "VertexArray.h"

#include <GL/glew.h>

VertexArray::VertexArray() {
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &this->VAO);
}

void VertexArray::bind() const {
	glBindVertexArray(this->VAO);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}