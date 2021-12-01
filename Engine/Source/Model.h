#pragma once

#include "Resource.h"
#include <vector>

typedef unsigned int uint;

struct ModelParameters
{
	bool flippedUvs = false;
	bool optimizedMesh = false;
	bool hasNormals = false;
	bool triangulated = false;
};

class Model : public Resource
{
public:
	Model(uint uid, std::string& assets, std::string& library);

	~Model();

	void Load() override;

	void DrawOnEditor() override;

	inline void SetMeshes(std::vector<uint>& meshesUids) { meshes = meshesUids; }

private:
	void Reimport();

	ModelParameters parameters;

	std::vector<uint> meshes;
};