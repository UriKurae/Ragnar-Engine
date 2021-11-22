#pragma once

#include <string>

#include "MathGeoLib/src/Math/float2.h"
#include "MathGeoLib/src/Math/float3.h"

#include "assimp/mesh.h"
#include "assimp/scene.h"

class JsonParsing;
class Mesh;

namespace MeshImporter
{
	void ImportMesh(const aiMesh* mesh, const aiScene* scene, JsonParsing& json, std::string& path);
	void SaveMesh(std::string& name, std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float3>& normals, std::vector<float2>& texCoords);
	Mesh* LoadMesh(const char* path);
}