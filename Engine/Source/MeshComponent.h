#pragma once

#include "Component.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "TextureBuffer.h"
#include "MathGeoLib/src/MathGeoLib.h"

#include <vector>

typedef unsigned int GLuint;
typedef unsigned char GLubyte;
class TransformComponent;

struct Texture
{
	GLuint id;
	const char* type;
	int width;
	int height;
	GLubyte* data;
};

class MeshComponent : public Component
{
public:
	MeshComponent() : ebo(nullptr), vbo(nullptr), texBuffer(nullptr), tbo(0), texture({}) {}
	MeshComponent(std::vector<float3> vert, std::vector<unsigned int> ind, Texture tex, std::vector<float2> texCoord);
	~MeshComponent();

	void Draw() override;
	
	inline void SetTransform(TransformComponent* trans) { transform = trans; }

private:
	VertexBuffer* vbo;
	IndexBuffer* ebo;
	TextureBuffer* texBuffer;
	Texture texture;
	unsigned int tbo;

	std::vector<float3> vertices;
	std::vector<unsigned int> indices;
	std::vector<float2> texCoords;

	TransformComponent* transform;
};