#pragma once

#include "MathGeoLib/src/MathGeoLib.h"

typedef unsigned int GLuint;
typedef unsigned short GLushort;
typedef float GLfloat;

class Primitive
{
public:
	Primitive() {}
	virtual ~Primitive() {}

	virtual void Draw() {}

	inline GLuint GetIndexBuffer() { return indexBuffer; }

protected:
	float3 transform;
	float3 rotate;
	float3 scale;

	GLuint indexBuffer;
};

class PCube : public Primitive
{
public:
	PCube(float3 t, float3 r, float3 s);

	~PCube();

	void Draw() override;

private:
	std::vector<GLfloat> indexVertex;
	std::vector<GLuint> indices;
};

class PPlane : public Primitive
{
public:
	PPlane(float3 t, float3 r, float3 s);

	~PPlane();

	void Draw() override;
};

class PCylinder : public Primitive
{
public:

	PCylinder(int sectCount, float h, float rad);
	~PCylinder();

	std::vector<float> GetUnitCircleVertices();

	void Draw() override;

	void BuildVerticesSmooth();

public:

	int sectorCount;
	float height;
	float radius;

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texCoords;
	std::vector<GLuint> indices;
};

class PPyramid : public Primitive
{
public:
	PPyramid(float3 t, float3 r, float3 s);

	~PPyramid();

	void Draw() override;

private:
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
};

class PSphere : public Primitive
{
public:
	PSphere(float radius, unsigned int rings, unsigned int sectors);

	~PSphere();

	void Draw() override;

private:
	std::vector<GLushort> indices;
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texCoords;
};