#pragma once
#include <vector>
#include "assimp/scene.h"

class JsonParsing;
struct Vertex;
struct ModelParameters;

typedef unsigned int uint;

namespace MeshImporter
{
	void ReImportMesh(const aiMesh* mesh, const aiScene* scene, JsonParsing& json, std::string& library, std::string& path, ModelParameters& data);
	void ImportMesh(const aiMesh* mesh, const aiScene* scene, JsonParsing& json, std::string& path, std::vector<uint>& uids);
	void SaveMesh(std::string& name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void LoadMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::string& path);

	void CreateMetaMesh(std::string& library, std::string& assets, uint uid);
}