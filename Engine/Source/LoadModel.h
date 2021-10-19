#pragma once

#include <string>

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"
#include "MeshComponent.h"

class GameObject;
class TransformComponent;

class LoadModel
{
public:
	static LoadModel* GetInstance();
	~LoadModel();

	void LoadingModel(std::string path);
	TransformComponent* LoadingTransform(aiNode* node);

	void ProcessNode(aiNode* node, const aiScene* scene, GameObject* object);
	MeshComponent* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	Texture LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName);

private:
	LoadModel() {}
	LoadModel(const LoadModel&) {}

	static LoadModel* instance;

	std::string directory;
};