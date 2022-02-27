#include "MeshImporter.h"

#include "Application.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "Component.h"
#include "TextureImporter.h"
#include "Mesh.h"
#include "Model.h"
#include "AnimationImporter.h"

#include "Vertex.h"

#include "Algorithm/Random/LCG.h"

#include "Globals.h"

#include "Profiling.h"

void MeshImporter::ReImportMesh(const aiMesh* mesh, const aiScene* scene, JsonParsing& json, std::string& library, std::string& path, ModelParameters& data)
{
	RG_PROFILING_FUNCTION("Importing mesh");

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	//std::vector<float3> norms;
	//std::vector<float2> texCoords;

	unsigned int numBones = mesh->mNumBones;
	std::vector<unsigned int> bonesUid;

	int numVertices = mesh->mNumVertices;
	int numFaces = mesh->mNumFaces;

	//vertices.reserve(numVertices);
	indices.reserve(numFaces * 3);
	//texCoords.reserve(numVertices);

	for (unsigned int i = 0; i < numVertices; ++i)
	{
		Vertex vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		//float3 normals;
		if (data.normals && mesh->HasNormals())
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
			//norms.push_back(normals);
		}

		//float2 coords;
		if (mesh->mTextureCoords[0])
		{
			vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
		//texCoords.push_back(coords);
	}

	for (unsigned int i = 0; i < numFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	for (unsigned int i = 0; i < numBones; i++)
	{
		AnimationImporter::ReImportBones(path, mesh->mBones[i], json, library, bonesUid);
	}

	// TODO: Check the re-import when animation is completed
	//SaveMesh(library, vertices, indices/*, norms, texCoords*/, bonesUid);

	JSON_Array* array = json.SetNewJsonArray(json.GetRootValue(), "Components");
	JsonParsing parse = JsonParsing();
	parse.SetNewJsonNumber(parse.ValueToObject(parse.GetRootValue()), "Type", (int)ComponentType::MESH_RENDERER);
	parse.SetNewJsonString(parse.ValueToObject(parse.GetRootValue()), "Mesh Path", library.c_str());

	json.SetValueToArray(array, parse.GetRootValue());

	if (mesh->mMaterialIndex >= 0)
	{
		DEBUG_LOG("Processing material...");

		JsonParsing mat = JsonParsing();
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		TextureImporter::ImportTexture(material, aiTextureType_DIFFUSE, "texture_diffuse", mat, path);
		DEBUG_LOG("Material loading completed!");
		json.SetValueToArray(array, mat.GetRootValue());
	}
}

void MeshImporter::ImportMesh(const aiMesh* mesh, const aiScene* scene, JsonParsing& json, std::string& path, std::vector<uint>& uids)
{
	RG_PROFILING_FUNCTION("Importing mesh");

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int numBones = mesh->mNumBones;
	std::vector<unsigned int> bonesUid;

	//std::vector<float3> vertices;
	//std::vector<float3> norms;
	//std::vector<float2> texCoords;

	int numVertices = mesh->mNumVertices;
	int numFaces = mesh->mNumFaces;

	//vertices.reserve(numVertices * 2);
	//indices.reserve(numFaces * 3);
	
	//vertices.reserve(numVertices);
	//texCoords.reserve(numVertices);

	for (unsigned int i = 0; i < numVertices; ++i)
	{
		Vertex vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		for (int i = 0; i < 4; ++i)
		{
			vertex.boneIDs[i] = -1;
			vertex.weights[i] = 0.0f;
		}

		//float3 normals;
		if (mesh->HasNormals())
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}

		//float2 coords;
		if (mesh->mTextureCoords[0])
		{
			vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
		//norms.push_back(normals);
		//texCoords.push_back(coords);
	}

	for (unsigned int i = 0; i < numFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	std::map<std::string, BoneInfo> bones;
	ExtractBonesAndWeights(vertices, mesh, scene, bones);

	//for (unsigned int i = 0; i < numBones; i++)
	//{
	//	AnimationImporter::ImportBones(path, mesh->mBones[i], json, uids, bonesUid);
	//}

	std::string meshName;
	std::string assetsPath(path);
	std::string name("__");
	name += mesh->mName.C_Str();

	assetsPath.insert(assetsPath.find_last_of("."), name.c_str());

	uint uid = ResourceManager::GetInstance()->CreateResource(ResourceType::MESH, assetsPath, meshName);
	uids.push_back(uid);
	
	SaveMesh(meshName, vertices, indices/*, norms, texCoords*/, bones);

	JSON_Array* array = json.SetNewJsonArray(json.GetRootValue(), "Components");
	JsonParsing parse = JsonParsing();
	parse.SetNewJsonNumber(parse.ValueToObject(parse.GetRootValue()), "Type", (int)ComponentType::MESH_RENDERER);
	parse.SetNewJsonString(parse.ValueToObject(parse.GetRootValue()), "Mesh Path", meshName.c_str());

	json.SetValueToArray(array, parse.GetRootValue());

	if (mesh->mMaterialIndex >= 0)
	{
		DEBUG_LOG("Processing material...");

		JsonParsing mat = JsonParsing();
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		TextureImporter::ImportTexture(material, aiTextureType_DIFFUSE, "texture_diffuse", mat, path);
		DEBUG_LOG("Material loading completed!");
		json.SetValueToArray(array, mat.GetRootValue());
	}
}

void MeshImporter::SaveMesh(std::string& name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::map<std::string, BoneInfo>& bones)
{
	unsigned int ranges[3] = { vertices.size(), indices.size(), bones.size() };

	uint size = sizeof(ranges) + sizeof(Vertex) * vertices.size() + sizeof(unsigned int) * indices.size() + sizeof(unsigned int) * bones.size() + sizeof(std::string) * bones.size() + sizeof(float3) * 2 * bones.size() + sizeof(Quat) * bones.size();	char* fileBuffer = new char[size];
	char* cursor = fileBuffer;

	unsigned int bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	bytes = sizeof(Vertex) * vertices.size();
	memcpy(cursor, vertices.data(), bytes);
	cursor += bytes;

	bytes = sizeof(unsigned int) * indices.size();
	memcpy(cursor, indices.data(), bytes);
	cursor += bytes;

	for (std::map<std::string, BoneInfo>::iterator it = bones.begin(); it != bones.end(); ++it)
	{
		std::string name = (*it).first;
		BoneInfo info = (*it).second;

		bytes = sizeof(unsigned int);
		unsigned int size = name.size();
		memcpy(cursor, &size, bytes);
		cursor += bytes;

		const char* str = name.c_str();
		bytes = strlen(str);
		memcpy(cursor, &str[0], bytes);
		cursor += bytes;

		// WARNING: If you uncomment this, it crashes
		SerializeBoneData(info, &cursor);

		//bytes = sizeof(info);
		//memcpy(cursor, &buf, bytes);
		//cursor += bytes;
		
		//bytes = sizeof(info);
		//memcpy(cursor, &info, bytes);
		//cursor += bytes;
	}
	//bytes = sizeof(std::map<std::string, BoneInfo>) * bones.size();
	//memcpy(cursor, &bones, bytes);

	if (app->fs->Save(name.c_str(), fileBuffer, size) > 0)
		DEBUG_LOG("Mesh %s saved succesfully", name.c_str());

	//RELEASE_ARRAY(fileBuffer);


	//unsigned int header[2] = { vertices.size(), indices.size() };

	//uint size = sizeof(header) + sizeof(Vertex) * vertices.size() + sizeof(unsigned int) * indices.size();

	//char* buffer = new char[size];
	//char* cursor = buffer;

	//uint bytes = sizeof(header);
	//memcpy(cursor, header, bytes);
	//cursor += bytes;

	//bytes = sizeof(Vertex) * vertices.size();
	//memcpy(cursor, vertices.data(), bytes);
	//cursor += bytes;

	//bytes = sizeof(unsigned int) * indices.size();
	//memcpy(cursor, indices.data(), bytes);
	//cursor += bytes;

	////bytes = sizeof(float3) * normals.size();
	////memcpy(cursor, normals.data(), bytes);
	////cursor += bytes;
	////
	////bytes = sizeof(float2) * texCoords.size();
	////memcpy(cursor, texCoords.data(), bytes);
	////cursor += bytes;

	//if (app->fs->Save(name.c_str(), buffer, size) > 0)
	//	DEBUG_LOG("Mesh %s saved succesfully", name.c_str());

	//RELEASE_ARRAY(buffer);
}

void MeshImporter::LoadMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::map<std::string, BoneInfo>& bones, std::string& path)
{
	char* buffer = nullptr;

	if (app->fs->Load(path.c_str(), &buffer) > 0)
	{
		char* cursor = buffer;
		unsigned int ranges[3];

		unsigned int bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);
		cursor += bytes;

		vertices.resize(ranges[0]);
		indices.resize(ranges[1]);

		// Load vertices
		bytes = sizeof(Vertex) * vertices.size();
		memcpy(vertices.data(), cursor, bytes);
		cursor += bytes;

		// Load indices
		bytes = sizeof(unsigned int) * indices.size();
		memcpy(indices.data(), cursor, bytes);
		cursor += bytes;

		// Load bones
		for (int i = 0; i < ranges[2]; ++i)
		{
			unsigned int stringSize;
			std::string name;
			BoneInfo info;
			
			bytes = sizeof(unsigned int);
			memcpy(&stringSize, cursor, bytes);
			cursor += bytes;

			// WARNING: If you uncomment this, it crashes
			bytes = stringSize;
			name.resize(stringSize);
			memcpy(name.data(), cursor, bytes);
			cursor += bytes;

			bytes = sizeof(int);
			memcpy(&info.id, cursor, bytes);
			cursor += bytes;

			float3 position;
			Quat rotation;
			float3 scale;

			bytes = sizeof(float3);
			memcpy(&position, cursor, bytes);
			cursor += bytes;

			bytes = sizeof(Quat);
			memcpy(&rotation, cursor, bytes);
			cursor += bytes;

			bytes = sizeof(float3);
			memcpy(&scale, cursor, bytes);
			cursor += bytes;

			info.offset = info.offset.FromTRS(position, rotation, scale);

			bones[name] = info;
		}
		RELEASE_ARRAY(buffer);
	}
	else
		DEBUG_LOG("Mesh file not found!");

	RELEASE_ARRAY(buffer);
}

void MeshImporter::CreateMetaMesh(std::string& library, std::string& assets, uint uid)
{
	JsonParsing metaFile;

	metaFile.SetNewJsonString(metaFile.ValueToObject(metaFile.GetRootValue()), "Assets Path", assets.c_str());
	metaFile.SetNewJsonNumber(metaFile.ValueToObject(metaFile.GetRootValue()), "Uuid", uid);

	char* buffer = nullptr;
	size_t size = metaFile.Save(&buffer);

	app->fs->Save(library.c_str(), buffer, size);

	RELEASE_ARRAY(buffer);
}

void MeshImporter::ExtractBonesAndWeights(std::vector<Vertex>& vertices, const aiMesh* mesh, const aiScene* scene, std::map<std::string, BoneInfo>& bones)
{
	int numBones = 0;
	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();

		if (bones.find(boneName) == bones.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = numBones;
			newBoneInfo.offset = FromAssimpMatrixToMathGeoLib(mesh->mBones[boneIndex]->mOffsetMatrix);
			bones[boneName] = newBoneInfo;
			boneID = numBones;
			numBones++;
		}
		else
		{
			boneID = bones[boneName].id;
		}

		aiVertexWeight* weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			SetBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

float4x4 MeshImporter::FromAssimpMatrixToMathGeoLib(aiMatrix4x4& matrix)
{
	float4x4 mathMatrix;
	aiQuaternion quat;
	aiVector3D pos;
	aiVector3D sca;
	matrix.Decompose(sca, quat, pos);
	
	float3 position = { pos.x, pos.y, pos.z };
	float3 scale = { sca.x, sca.y, sca.z };
	Quat quaternion = { quat.x, quat.y, quat.z, quat.w };

	return mathMatrix.FromTRS(position, quaternion, scale);
}

void MeshImporter::SetBoneData(Vertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < 4; ++i)
	{
		if (vertex.boneIDs[i] < 0)
		{
			vertex.weights[i] = weight;
			vertex.boneIDs[i] = boneID;
			break;
		}
	}
}

void MeshImporter::SerializeBoneData(BoneInfo& info, char** buffer)
{
	unsigned int bytes = sizeof(info.id);
	memcpy(*buffer, &info.id, bytes);
	*buffer += bytes;

	float3 position;
	Quat rotation;
	float3 scale;
	info.offset.Decompose(position, rotation, scale);

	bytes = sizeof(float3);
	memcpy(*buffer, &position, bytes);
	*buffer += bytes;

	bytes = sizeof(Quat);
	memcpy(*buffer, &rotation, bytes);
	*buffer += bytes;

	bytes = sizeof(float3);
	memcpy(*buffer, &scale, bytes);
	*buffer += bytes;
}