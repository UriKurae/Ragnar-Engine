#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Math/float3.h"

typedef unsigned int GLuint;
typedef unsigned short GLushort;
typedef float GLfloat;
typedef unsigned char GLubyte;

class Primitive
{
public:
	Primitive() : vao(0), index(0), vertex(0), showAxis(0) {}
	virtual ~Primitive() {}

	virtual void Draw() {}
	//inline GLuint GetIndexBuffer() { return indexBuffer; }

	void DrawAxis();

protected:
	float3 transform;
	float3 rotate;
	float3 scale;

	VertexBuffer* vertex;
	IndexBuffer* index;

	unsigned int vao;

	bool showAxis;
};

class PGrid : public Primitive
{
public:
	PGrid(float w, float h);
	~PGrid();

	void Draw() override;

	void CreateGrid();

private:
	std::vector<float3> vertices;

	float width;
	float height;
};