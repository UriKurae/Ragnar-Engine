#pragma once

#include "BufferLayout.h"

class VertexBuffer
{
public:
	VertexBuffer() : buffer(0), size(0) {}
	VertexBuffer(const void* data, unsigned int s);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetSize() const { return size; }

	inline const BufferLayout& GetLayout() const { return layout; }

	// Make sure to do this BEFORE adding the VertexBuffer into the VertexArray.
	inline void SetLayout(const BufferLayout& l) { layout = l; }

private:
	unsigned int buffer;
	unsigned int size;

	BufferLayout layout;
};