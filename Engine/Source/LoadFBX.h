#pragma once

#include "Mesh.h"

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