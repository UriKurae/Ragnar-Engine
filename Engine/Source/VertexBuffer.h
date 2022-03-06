#pragma once

#include "BufferLayout.h"
#include "Math/float3.h"
#include "Math/float2.h"

struct Vertex
{
	float3 position;
	float3 normal;
	float2 texCoords;
};

class VertexBuffer
{
public:
	VertexBuffer() : buffer(0), count(0) {}
	VertexBuffer(const void* data, unsigned int c);
	VertexBuffer(const std::vector<Vertex>& vertices);
	void SetData(const std::vector<Vertex>&);

	~VertexBuffer();

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