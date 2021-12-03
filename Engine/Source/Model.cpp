#include "Model.h"

#include "Globals.h"
#include "ModelImporter.h"

#include "ResourceManager.h"
#include "Mesh.h"

#include "Imgui/imgui.h"

#include "Profiling.h"

Model::Model(uint uid, std::string& assets, std::string& library) : parameters({}), Resource(uid, ResourceType::MODEL, assets, library)
{
	std::string metaPath = MODELS_FOLDER + std::string("model_") + std::to_string(uid) + ".meta";
	ModelImporter::CreateMetaModel(metaPath, parameters, assets, uid);
	//ModelImporter::ImportModel(assets, parameters);
}

Model::~Model()
{
}

void Model::Load()
{
	ModelImporter::LoadModel(libraryPath);
}

void Model::DrawOnEditor()
{
	if (ImGui::CollapsingHeader("Model Import Settings"))
	{
		ImGui::Checkbox("Optimize Mesh", &parameters.optimizedMesh);
		ImGui::Checkbox("Has Normals", &parameters.normals);
		ImGui::Checkbox("Flipped UVs", &parameters.flippedUvs);
		ImGui::Checkbox("Triangulate", &parameters.triangulated);
		
		if (ImGui::Button("Apply Changes")) Reimport();
	}
}

void Model::Reimport()
{
	std::string metaPath = MODELS_FOLDER + std::string("model_") + std::to_string(uid) + ".meta";
	ModelImporter::CreateMetaModel(metaPath, parameters, assetsPath, uid);
	ModelImporter::ReImport(assetsPath, libraryPath, parameters);

	for (int i = 0; i < meshes.size(); ++i)
	{
		std::shared_ptr<Mesh> mesh = std::static_pointer_cast<Mesh>(ResourceManager::GetInstance()->GetResource(meshes[i]));
		mesh->Reimport(parameters);
	}
}