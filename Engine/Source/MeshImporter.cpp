#include "MeshImporter.h"

#include "Application.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "Component.h"
#include "TextureImporter.h"
#include "Mesh.h"
#include "Model.h"
#include "MathGeoLib/src/Algorithm/Random/LCG.h"

#include "Globals.h"

#include "Profiling.h"

void MeshImporter::ReImportMesh(const aiMesh* mesh, const aiScene* scene, JsonParsing& json, std::string& library, std::string& path, ModelParameters& data)
{
	RG_PROFILING_FUNCTION("Importing mesh");

	std::vector<float3> vertices;
	std::vector<float3> norms;
	std::vector<unsigned int> indices;
	std::vector<float2> texCoords;

	int numVertices = mesh->mNumVertices;
	int numFaces = mesh->mNumFaces;

	vertices.reserve(numVertices);
	indices.reserve(numFaces * 3);
	texCoords.reserve(numVertices);

	for (unsigned int i = 0; i < numVertices; ++i)
	{
		float3 vertex;
		vertex.x = mesh->mVertices[i].x;
		vertex.y = mesh->mVertices[i].y;
		vertex.z = mesh->mVertices[i].z;

		float3 normals;
		if (data.normals && mesh->HasNormals())
		{
			normals.x = mesh->mNormals[i].x;
			normals.y = mesh->mNormals[i].y;
			normals.z = mesh->mNormals[i].z;
			norms.push_back(normals);
		}

		float2 coords;
		if (mesh->mTextureCoords[0])
		{
			coords.x = mesh->mTextureCoords[0][i].x;
			coords.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
		texCoords.push_back(coords);
	}

	for (unsigned int i = 0; i < numFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	SaveMesh(library, vertices, indices, norms, texCoords);

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

	std::vector<float3> vertices;
	std::vector<float3> norms;
	std::vector<unsigned int> indices;
	std::vector<float2> texCoords;

	int numVertices = mesh->mNumVertices;
	int numFaces = mesh->mNumFaces;

	vertices.reserve(numVertices);
	indices.reserve(numFaces * 3);
	texCoords.reserve(numVertices);

	for (unsigned int i = 0; i < numVertices; ++i)
	{
		float3 vertex;
		vertex.x = mesh->mVertices[i].x;
		vertex.y = mesh->mVertices[i].y;
		vertex.z = mesh->mVertices[i].z;

		float3 normals;
		if (mesh->HasNormals())
		{
			normals.x = mesh->mNormals[i].x;
			normals.y = mesh->mNormals[i].y;
			normals.z = mesh->mNormals[i].z;
		}

		float2 coords;
		if (mesh->mTextureCoords[0])
		{
			coords.x = mesh->mTextureCoords[0][i].x;
			coords.y = mesh->mTextureCoords[0][i].y;
		}

		norms.push_back(normals);
		vertices.push_back(vertex);
		texCoords.push_back(coords);
	}

	for (unsigned int i = 0; i < numFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	std::string meshName;
	std::string assetsPath(path);
	std::string name("__");
	name += mesh->mName.C_Str();

	assetsPath.insert(assetsPath.find_last_of("."), name.c_str());

	uint uid = ResourceManager::GetInstance()->CreateResource(ResourceType::MESH, assetsPath, meshName);
	uids.push_back(uid);
	
	SaveMesh(meshName, vertices, indices, norms, texCoords);

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

void MeshImporter::SaveMesh(std::string& name, std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float3>& normals, std::vector<float2>& texCoords)
{
	unsigned int header[4] = { vertices.size(), indices.size(), normals.size(), texCoords.size() };

	uint size = sizeof(header) + sizeof(float3) * vertices.size() + sizeof(unsigned int) * indices.size() + sizeof(float3) * normals.size() + sizeof(float2) * texCoords.size();

	char* buffer = new char[size];
	char* cursor = buffer;

	uint bytes = sizeof(header);
	memcpy(cursor, header, bytes);
	cursor += bytes;

	bytes = sizeof(float3) * vertices.size();
	memcpy(cursor, vertices.data(), bytes);
	cursor += bytes;

	bytes = sizeof(unsigned int) * indices.size();
	memcpy(cursor, indices.data(), bytes);
	cursor += bytes;

	bytes = sizeof(float3) * normals.size();
	memcpy(cursor, normals.data(), bytes);
	cursor += bytes;

	bytes = sizeof(float2) * texCoords.size();
	memcpy(cursor, texCoords.data(), bytes);
	cursor += bytes;

	if (app->fs->Save(name.c_str(), buffer, size) > 0)
		DEBUG_LOG("Mesh %s saved succesfully", name.c_str());

	RELEASE_ARRAY(buffer);
}

void MeshImporter::LoadMesh(std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float3>& normals, std::vector<float2>& texCoords, std::string& path)
{
	char* buffer = nullptr;

	if (app->fs->Load(path.c_str(), &buffer) > 0)
	{
		char* cursor = buffer;
		unsigned int* header = new unsigned int[4];

		// Loading header information
		uint bytes = sizeof(header) * sizeof(unsigned int);
		memcpy(header, buffer, bytes);
		cursor += bytes;

		// Setting information
		vertices.resize(header[0]);
		indices.resize(header[1]);
		normals.resize(header[2]);
		texCoords.resize(header[3]);

		// Loading vertices
		bytes = sizeof(float3) * vertices.size();
		memcpy(&vertices[0], cursor, bytes);
		cursor += bytes;

		// Loading indices
		bytes = sizeof(unsigned int) * indices.size();
		memcpy(indices.data(), cursor, bytes);
		cursor += bytes;

		// Loading normals
		bytes = sizeof(float3) * normals.size();
		memcpy(normals.data(), cursor, bytes);
		cursor += bytes;

		// Loading texture coordinates
		bytes = sizeof(float2) * texCoords.size();
		memcpy(texCoords.data(), cursor, bytes);

		RELEASE_ARRAY(header);
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