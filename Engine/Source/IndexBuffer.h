#pragma once

class IndexBuffer
{
public:
	IndexBuffer() : buffer(0), size(0) {}
	IndexBuffer(const unsigned int* data, unsigned int s);
	IndexBuffer(const unsigned short* data, unsigned int s);

	~IndexBuffer();

	void Bind() const;

	void Unbind() const;

	inline unsigned int GetSize() const { return size; }

private:
	unsigned int buffer;
	unsigned int size;
};