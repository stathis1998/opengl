#pragma once

class VertexArray {
private:
	unsigned int VAO;
public:
	VertexArray();
	~VertexArray();

	void bind() const;
	void unbind() const;

	void addAttribute(int index, int size, int type, bool normalized, int stride, const void* pointer);
};

