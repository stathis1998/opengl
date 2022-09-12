#pragma once

class VertexBuffer {
private:
	unsigned int VBO;
public:
	VertexBuffer(const void* data, const unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
};

