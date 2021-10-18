#pragma once

#include "Mesh.h"

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"

#include <vector>

class GameObject;

class Model
{
public:
	Model() {}
	Model(const char* path);
	~Model();

	void Draw();

private:
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene, GameObject* object);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	Texture LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName);

private:

	std::vector<Mesh> meshes;
	std::string directory;
};