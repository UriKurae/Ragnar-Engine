#pragma once

#include <string>
#include <vector>

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"

#include "MathGeoLib/src/Math/float2.h"
#include "MathGeoLib/src/Math/float3.h"

class GameObject;
class TransformComponent;
class MeshComponent;
class MaterialComponent;

typedef uint64_t Uint64;

class LoadModel
{
public:
	static LoadModel* GetInstance();
	static void ReleaseInstance();
	~LoadModel();

	void ImportModel(std::string& path);
	void LoadingModel(std::string& path);
	void LoadingTransform(aiNode* node, GameObject* obj);

	void ProcessNode(aiNode* node, const aiScene* scene, GameObject* object);
	void ProcessNode2(aiNode* node, const aiScene* scene);
	MeshComponent* ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* object);
	void ProcessMesh2(aiMesh* mesh, const aiScene* scene);
	MaterialComponent* LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName);

	Uint64 SaveMesh(const char* name, std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float3>& normals, std::vector<float2>& texCoords);
	void LoadMesh(const char* name, MeshComponent* mesh);
private:
	LoadModel() {}
	LoadModel(const LoadModel&) {}

	static LoadModel* instance;

	std::string directory;
};