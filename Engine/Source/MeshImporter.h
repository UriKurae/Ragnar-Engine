#pragma once

#include <string>

#include "MathGeoLib/include/Math/float2.h"
#include "MathGeoLib/include/Math/float3.h"

#include "assimp/mesh.h"
#include "assimp/scene.h"

class JsonParsing;
class Mesh;
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