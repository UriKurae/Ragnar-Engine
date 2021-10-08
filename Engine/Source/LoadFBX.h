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