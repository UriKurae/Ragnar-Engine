#pragma once

#include "MathGeoLib/src/MathGeoLib.h"
#include "MeshComponent.h"

typedef unsigned char GLubyte;
typedef unsigned int GLuint;

struct Vertex
{
	float3 position;
	float3 normal;
	float2 texCoords;
};

//struct Texture
//{
//	GLuint id;
//	const char* type;
//	int width;
//	int height;
//	GLubyte* data;
//};

class Mesh
{
public:
	Mesh() : vao(0), vbo(0), ebo(0){}
	Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind, Texture text, std::vector<float2> texCoord);

	void Draw();

public:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Texture textures;
	std::vector<float2> texCoords;

private:
	void SetupMesh();
	void CreateCheckerImage();

private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int tbo;
	unsigned int texId;

	GLubyte checkerImage[64][64][4];
};