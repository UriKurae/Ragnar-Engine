#pragma once

#include <string>

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"

#include "JsonParsing.h"

class GameObject;
class ModelParameters;

namespace ModelImporter
{
	void ImportModel(std::string& path);
	void SaveModel(std::string& path, JsonParsing& json);
	void LoadModel(std::string& path);

	void CreateImportSettings();

	void ProcessNode(aiNode* node, const aiScene* scene, JsonParsing& nodeJ, JSON_Array* json, std::string& path);
	void CreatingModel(JsonParsing& json, JSON_Array* array, GameObject* go);
	void CreateMetaModel(std::string& path, ModelParameters& data);
}