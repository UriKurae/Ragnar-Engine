#pragma once

#include "MathGeoLib/src/MathGeoLib.h"

struct Mesh
{
	unsigned int indexId = 0;
	unsigned int numIndex = 0;
	unsigned int* indices;

	unsigned int vertexId = 0;
	unsigned int numVertex = 0;
	float3* vertices;

	unsigned int texCoordId = 0;
	unsigned int numTexCoord = 0;
	float2* textures;

	unsigned int normalsId = 0;
	unsigned int numNormals = 0;
	float3* normals;

	unsigned int colorsId = 0;
	unsigned int numColors = 0;
	float3* colors;	
};

class LoadFBX
{
public:
	LoadFBX();

	~LoadFBX();

	void LoadMesh(const char* path);

	inline Mesh& GetMesh() { return ourMesh; }

private:
	Mesh ourMesh;
};