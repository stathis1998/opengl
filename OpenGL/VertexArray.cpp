#include "VertexArray.h"

#include <GL/glew.h>

VertexArray::VertexArray() {
	glGenVertexArrays(1, &this->VAO);
	this->bind();
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &this->VAO);
}

void VertexArray::bind() const {
	glBindVertexArray(this->VAO);
}

void VertexArray::addAttribute(int index, int size, int type, bool normalized, int stride, const void* pointer) {
	glVertexAttribPointer(index, size, type, normalized, stride, (void*)pointer);
	glEnableVertexAttribArray(index);
}