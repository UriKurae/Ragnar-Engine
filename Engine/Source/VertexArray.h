#pragma once
#include "VertexBuffer.h"
#include <vector>

class IndexBuffer;

class VertexArray
{
public:
	VertexArray();
	virtual ~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(VertexBuffer& vertexBuf);
	void SetIndexBuffer(IndexBuffer& indexBuf); // Set and not Add because you can only have 1 IndexBuffer

	inline std::vector<VertexBuffer*>& GetVertexBuffers() { return vertexBuffers; }
	inline IndexBuffer* GetIndexBuffer() { return indexBuffer; }

private:
	uint32_t vao;

	std::vector<VertexBuffer*> vertexBuffers;
	IndexBuffer* indexBuffer;
};