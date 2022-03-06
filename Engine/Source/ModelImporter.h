#pragma once
#include <map>
#include <vector>

#include "Bone.h"
#include "JsonParsing.h"

class GameObject;
class ModelParameters;
class aiScene;
class aiNode;

typedef unsigned int uint;

namespace ModelImporter
{
	void ReImport(std::string& assetsPath, std::string& library, ModelParameters& parameters);
	void ImportModel(std::string& path);
	void SaveModel(std::string& path, JsonParsing& json);
	void LoadModel(std::string& path);

	void ProcessNode(aiNode* node, const aiScene* scene, JsonParsing& nodeJ, JSON_Array* json, std::string& path, std::vector<uint>& uids, std::map<std::string, BoneInfo>& bones);
	void ReProcessNode(aiNode* node, const aiScene* scene, JsonParsing& nodeJ, JSON_Array* json, std::string& path, ModelParameters& data);
	void CreatingModel(JsonParsing& json, JSON_Array* array, GameObject* go);
	void CreateMetaModel(std::string& path, ModelParameters& data, std::string& assets, uint uid);
}