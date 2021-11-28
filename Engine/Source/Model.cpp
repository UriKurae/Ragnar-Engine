#include "Model.h"

#include "ModelImporter.h"

#include "Profiling.h"

Model::Model(uint uid, std::string& assets, std::string& library) : Resource(uid, ResourceType::MODEL, assets, library)
{
}

Model::~Model()
{
}

void Model::Load()
{
	ModelImporter::LoadModel(libraryPath);
}