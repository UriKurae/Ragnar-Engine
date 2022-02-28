#pragma once

#include "BufferLayout.h"
#include "Vertex.h"

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int c);
	VertexBuffer(const std::vector<Vertex>& vertices);
	~VertexBuffer();

	void SetData(const std::vector<Vertex>&);
	void SetData(float* vertices);


	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return count; }

	inline const BufferLayout& GetLayout() const { return layout; }

	// Make sure to do this BEFORE adding the VertexBuffer into the VertexArray.
	inline void SetLayout(const BufferLayout& l) { layout = l; }

private:
	unsigned int buffer;
	unsigned int count;

	BufferLayout layout;
};