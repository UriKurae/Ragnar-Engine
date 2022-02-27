#include "MeshImporter.h"

#include "Application.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "Component.h"
#include "TextureImporter.h"
#include "Mesh.h"
#include "Model.h"

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

	SaveMesh(library, vertices, indices/*, norms, texCoords*/);

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

	std::string meshName;
	std::string assetsPath(path);
	std::string name("__");
	name += mesh->mName.C_Str();

	assetsPath.insert(assetsPath.find_last_of("."), name.c_str());

	uint uid = ResourceManager::GetInstance()->CreateResource(ResourceType::MESH, assetsPath, meshName);
	uids.push_back(uid);
	
	SaveMesh(meshName, vertices, indices/*, norms, texCoords*/);

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

void MeshImporter::SaveMesh(std::string& name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	unsigned int ranges[2] = { vertices.size(), indices.size() };

	uint size = sizeof(ranges) + sizeof(Vertex) * vertices.size() + sizeof(unsigned int) * indices.size();

	char* fileBuffer = new char[size];
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

	if (app->fs->Save(name.c_str(), fileBuffer, size) > 0)
		DEBUG_LOG("Mesh %s saved succesfully", name.c_str());

	delete[] fileBuffer;


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

void MeshImporter::LoadMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::string& path)
{
	char* buffer = nullptr;

	if (app->fs->Load(path.c_str(), &buffer) > 0)
	{
		//char* buffer = nullptr;

		char* cursor = buffer;
		unsigned int ranges[2];

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