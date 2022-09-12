#pragma once

class VertexArray {
private:
	unsigned int VAO;
public:
	VertexArray();
	~VertexArray();

	void bind() const;
	void unbind() const;
};

