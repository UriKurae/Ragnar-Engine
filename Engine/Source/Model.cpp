#include "Model.h"

#include "ModelImporter.h"

#include "Imgui/imgui.h"

#include "Profiling.h"

Model::Model(uint uid, std::string& assets, std::string& library) : parameters({}), Resource(uid, ResourceType::MODEL, assets, library)
{
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
		ImGui::Checkbox("Has Normals", &parameters.hasNormals);
		ImGui::Checkbox("Flipped UVs", &parameters.flippedUvs);
		ImGui::Checkbox("Triangulate", &parameters.triangulated);
		
		if (ImGui::Button("Apply Changes")) Reimport();
	}
}

void Model::Reimport()
{

}