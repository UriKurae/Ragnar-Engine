#pragma once

#include <string>
#include <map>

#include "MathGeoLib/src/Math/float2.h"
#include "MathGeoLib/src/Math/float3.h"

#include "assimp/mesh.h"
#include "assimp/scene.h"

class JsonParsing;
class Mesh;
struct Vertex;
struct ModelParameters;
struct BoneInfo;

typedef unsigned int uint;

namespace MeshImporter
{
	void ReImportMesh(const aiMesh* mesh, const aiScene* scene, JsonParsing& json, std::string& library, std::string& path, ModelParameters& data);
	void ImportMesh(const aiMesh* mesh, const aiScene* scene, JsonParsing& json, std::string& path, std::vector<uint>& uids);
	void SaveMesh(std::string& name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::map<std::string, BoneInfo>& bonesUid);
	void LoadMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::map<std::string, BoneInfo>& bones, std::string& path);

	void CreateMetaMesh(std::string& library, std::string& assets, uint uid);
	void ExtractBonesAndWeights(std::vector<Vertex>& vertices, const aiMesh* mesh, const aiScene* scene, std::map<std::string, BoneInfo>& bones);

	float4x4 FromAssimpMatrixToMathGeoLib(aiMatrix4x4& matrix);
	void SetBoneData(Vertex& vertex, int boneID, float weight);
}