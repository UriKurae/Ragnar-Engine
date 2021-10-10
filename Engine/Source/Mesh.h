#pragma once

#include "MathGeoLib/src/MathGeoLib.h"

struct Vertex
{
	float3 position;
	float3 normal;
	float2 texCoords;
};

struct Texture
{
	unsigned int id;
	const char* type;
};

class Mesh
{
public:
	Mesh() : vao(0), vbo(0), ebo(0){}
	Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind, std::vector<Texture> text);

	void Draw();

public:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

private:
	void SetupMesh();

private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
};