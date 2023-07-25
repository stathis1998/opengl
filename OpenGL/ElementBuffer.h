#pragma once

class ElementBuffer {
private:
	unsigned int EBO;
	unsigned int count;
public:
	ElementBuffer(const unsigned int* data, const unsigned int count);
	~ElementBuffer();

	void bind() const;

	inline unsigned int getCount() const { return this->count; }
};

