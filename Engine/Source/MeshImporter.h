#pragma once

#include <string>

#include "MathGeoLib/src/Math/float2.h"
#include "MathGeoLib/src/Math/float3.h"

#include "assimp/mesh.h"
#include "assimp/scene.h"

class JsonParsing;
class Mesh;
struct ModelParameters;

typedef unsigned int uint;

namespace MeshImporter
{
	void ReImportMesh(const aiMesh* mesh, const aiScene* scene, JsonParsing& json, std::string& library, std::string& path, ModelParameters& data);
	void ImportMesh(const aiMesh* mesh, const aiScene* scene, JsonParsing& json, std::string& path, std::vector<uint>& uids);
	void SaveMesh(std::string& name, std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float3>& normals, std::vector<float2>& texCoords);
	void LoadMesh(std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float3>& normals, std::vector<float2>& texCoords, std::string& path);

	void CreateMetaMesh(std::string& library, std::string& assets, uint uid);
}