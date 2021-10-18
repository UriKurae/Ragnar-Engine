#pragma once

#include "Component.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "MathGeoLib/src/MathGeoLib.h"

#include <vector>

class MeshComponent : public Component
{
public:
	MeshComponent() : ebo(nullptr), vbo(nullptr), texId(0), tbo(0) {}
	MeshComponent(std::vector<float3> vert, std::vector<unsigned int> ind, std::vector<float2> texCoord);
	~MeshComponent();

	void Draw();

private:
	VertexBuffer* vbo;
	IndexBuffer* ebo;
	unsigned int texId;
	unsigned int tbo;

	std::vector<float3> vertices;
	std::vector<unsigned int> indices;
	std::vector<float2> texCoords;
};