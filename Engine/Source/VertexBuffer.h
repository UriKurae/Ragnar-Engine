#pragma once

class VertexBuffer
{
public:
	VertexBuffer() : buffer(0), size(0) {}
	VertexBuffer(const void* data, unsigned int s);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetSize() const { return size; }

private:
	unsigned int buffer;
	unsigned int size;

};