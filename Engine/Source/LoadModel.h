#pragma once

#include <string>

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"

class GameObject;
class TransformComponent;

class LoadModel
{
public:
	static LoadModel* GetInstance();
	static void ReleaseInstance();
	~LoadModel();

	void LoadingModel(std::string path);
	void LoadingTransform(aiNode* node, GameObject* obj);

	void ProcessNode(aiNode* node, const aiScene* scene, GameObject* object);
	MeshComponent* ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* object);
	MaterialComponent* LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName);

private:
	LoadModel() {}
	LoadModel(const LoadModel&) {}

	static LoadModel* instance;

	std::string directory;
};