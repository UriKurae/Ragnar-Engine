#include "ModelImporter.h"

#include "Application.h"
#include "ModuleScene.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "Globals.h"
#include "MeshImporter.h"
#include "TextureImporter.h"

#include <stack>

#include "Profiling.h"

void ModelImporter::ImportModel(std::string& path)
{
	RG_PROFILING_FUNCTION("Importing Model");

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcessPreset_TargetRealtime_MaxQuality);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		DEBUG_LOG("ERROR ASSIMP %s", import.GetErrorString());
		return;
	}

	std::string p = path.substr(0, path.find_last_of('.'));
	p = p.substr(path.find_last_of('\\') + 1, p.length());
	p = p.substr(path.find_last_of('/') + 1, p.length());

	JsonParsing json = JsonParsing();
	JsonParsing child = json.SetChild(json.GetRootValue(), "Model");
	child.SetNewJsonString(child.ValueToObject(child.GetRootValue()), "Name", p.c_str());
	std::string root = "Childs" + p;
	JSON_Array* array = child.SetNewJsonArray(child.GetRootValue(), root.c_str());

	ProcessNode(scene->mRootNode, scene, child, array);

	SaveModel(p, json);
}

void ModelImporter::SaveModel(std::string& path, JsonParsing& json)
{
	char* buffer = nullptr;
	size_t size = json.Save(&buffer);

	std::string fileName = MODELS_FOLDER + path + ".rgmodel";

	app->fs->Save(fileName.c_str(), buffer, size);
}

void ModelImporter::LoadModel(std::string& path)
{
	RG_PROFILING_FUNCTION("Loading Model");
	char* buffer = nullptr;

	path = path.substr(0, path.find_last_of("."));
	path = path.substr(path.find_last_of("/") + 1, path.length());
	path = MODELS_FOLDER + path + ".rgmodel";

	app->fs->Load(path.c_str(), &buffer);

	if (buffer != nullptr)
	{
		JsonParsing json = JsonParsing(buffer);

		json = json.GetChild(json.GetRootValue(), "Model");

		GameObject* go = new GameObject();
		std::string name = json.GetJsonString("Name");
		go->SetName(name.c_str());

		name = "Childs" + name;
		CreatingModel(json, json.GetJsonArray(json.ValueToObject(json.GetRootValue()), name.c_str()), app->scene->GetRoot());
	}
}

void ModelImporter::ProcessNode(aiNode* node, const aiScene* scene, JsonParsing& nodeJ, JSON_Array* json)
{
	if (node == scene->mRootNode || node->mNumMeshes > 0)
	{
		JsonParsing jsonValue = JsonParsing();
		jsonValue.SetNewJsonString(jsonValue.ValueToObject(jsonValue.GetRootValue()), "Name", node->mName.C_Str());

		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			MeshImporter::ImportMesh(mesh, scene, jsonValue);
		}

		std::string name = "Childs" + std::string(node->mName.C_Str());
		JSON_Array* array = jsonValue.SetNewJsonArray(jsonValue.GetRootValue(), name.c_str());
		// Repeat the process until there's no more children
		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(node->mChildren[i], scene, jsonValue, array);
		}
		nodeJ.SetValueToArray(json, jsonValue.GetRootValue());
	}
	else
	{
		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(node->mChildren[i], scene, nodeJ, json);
		}
	}
}

void ModelImporter::CreatingModel(JsonParsing& json, JSON_Array* array, GameObject* go)
{
	size_t size = json.GetJsonArrayCount(array);
	for (int i = 0; i < size; ++i)
	{
		GameObject* newGo = new GameObject();
		newGo->CreateComponent(ComponentType::TRANSFORM);
		newGo->SetParent(go);
		go->AddChild(newGo);

		JsonParsing parsing = json.GetJsonArrayValue(array, i);
		std::string name = parsing.GetJsonString("Name");

		newGo->SetName(name.c_str());
		JSON_Array* arr = parsing.GetJsonArray(parsing.ValueToObject(parsing.GetRootValue()), "Components");
		size_t s = parsing.GetJsonArrayCount(arr);

		for (int j = 0; j < s; ++j)
		{
			JsonParsing component = parsing.GetJsonArrayValue(arr, j);
			switch ((ComponentType)(int)component.GetJsonNumber("Type"))
			{
			case ComponentType::MESH_RENDERER:
			{
				MeshComponent* mesh = (MeshComponent*)newGo->CreateComponent(ComponentType::MESH_RENDERER);
				//LoadMesh(component.GetJsonString("Mesh Path"), mesh);
				mesh->SetMesh(MeshImporter::LoadMesh(component.GetJsonString("Mesh Path")));
				break;
			}
			case ComponentType::MATERIAL:
			{
				MaterialComponent* material = (MaterialComponent*)newGo->CreateComponent(ComponentType::MATERIAL);
				material->SetTexture(TextureImporter::LoadTexture(component.GetJsonString("Texture Path")));
				break;
			}
			}
		}

		name = "Childs" + name;
		CreatingModel(parsing, parsing.GetJsonArray(parsing.ValueToObject(parsing.GetRootValue()), name.c_str()), newGo);
	}
}