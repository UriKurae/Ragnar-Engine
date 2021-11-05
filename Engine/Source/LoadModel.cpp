#include "LoadModel.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Globals.h"
#include "TextureLoader.h"
#include "FileSystem.h"

#include "TransformComponent.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"

#include "MathGeoLib/src/MathGeoLib.h"
#include "IL/il.h"
#include "IL/ilut.h"

#include "Profiling.h"

LoadModel* LoadModel::instance = nullptr;

LoadModel* LoadModel::GetInstance()
{
	if (instance == nullptr) instance = new LoadModel();

	return instance;
}

void LoadModel::ReleaseInstance()
{
	RELEASE(instance);
}

LoadModel::~LoadModel()
{
}

void LoadModel::ImportModel(std::string& path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		DEBUG_LOG("ERROR ASSIMP %s", import.GetErrorString());
		return;
	}
	directory = path.substr(5, path.find_last_of('\\'));

	std::string p = path.substr(0, path.find_last_of('.'));
	p = p.substr(path.find_last_of('\\') + 1, p.size());

	ProcessNode2(scene->mRootNode, scene);
}

void LoadModel::LoadingModel(std::string& path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		DEBUG_LOG("ERROR ASSIMP %s", import.GetErrorString());
		return;
	}
	directory = path.substr(5, path.find_last_of('\\'));
	
	std::string p = path.substr(0, path.find_last_of('.'));
	p = p.substr(path.find_last_of('\\') + 1, p.size());
	p = p.substr(path.find_last_of('/') + 1, p.size());
	GameObject* object = app->scene->CreateGameObject(nullptr);
	object->SetName(p.c_str());
	ProcessNode(scene->mRootNode, scene, object);
	object->SetTotalAABB();
}

void LoadModel::ProcessNode(aiNode* node, const aiScene* scene, GameObject* obj)
{
	LoadingTransform(node, obj);
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		obj->CreateComponent(ComponentType::MESH_RENDERER);
		MeshComponent* component = obj->GetComponent<MeshComponent>();
		LoadMesh(mesh->mName.C_Str(), component);
		aiString str;
		scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		MaterialComponent* material = TextureLoader::GetInstance()->LoadTexture(std::string(str.C_Str()));
		component->SetMaterial(material);
		obj->AddComponent(material);
		obj->SetName(node->mName.C_Str());
	}

	// Repeat the process until there's no more children
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		if (node->mChildren[i]->mNumMeshes > 0)
		{
			GameObject* object = app->scene->CreateGameObject(obj);
			ProcessNode(node->mChildren[i], scene, object);
		}
		else
		{
			ProcessNode(node->mChildren[i], scene, obj);
		}
	}
}

void LoadModel::ProcessNode2(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh2(mesh, scene);
	}

	// Repeat the process until there's no more children
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode2(node->mChildren[i], scene);
	}
}

MeshComponent* LoadModel::ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* object)
{
	RG_PROFILING_FUNCTION("Process Mesh");
	DEBUG_LOG("Processing mesh...");
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
		coords.x = mesh->mTextureCoords[0][i].x;
		coords.y = mesh->mTextureCoords[0][i].y;

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

	MaterialComponent* diffuse = nullptr;
	if (mesh->mMaterialIndex >= 0)
	{
		DEBUG_LOG("Processing material...");
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		TextureLoader::GetInstance()->ImportTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		//diffuse = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		
		//std::vector<Texture> specular = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		//textures.insert(textures.end(), specular.begin(), specular.end());
		DEBUG_LOG("Material loading completed!");
	}
	MeshComponent* m = (MeshComponent*)object->CreateComponent(ComponentType::MESH_RENDERER);

	if (mesh->HasNormals())
		m->SetMesh(vertices, indices, texCoords, norms);
	else
		m->SetMesh(vertices, indices, texCoords);

	//m->CreateAABB();
	if (diffuse)
	{
		m->SetMaterial(diffuse);
		object->AddComponent(diffuse);
	}

	DEBUG_LOG("Mesh loading completed!");
	return m;
}

void LoadModel::ProcessMesh2(aiMesh* mesh, const aiScene* scene)
{
	RG_PROFILING_FUNCTION("Process Mesh");
	DEBUG_LOG("Processing mesh...");
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
		coords.x = mesh->mTextureCoords[0][i].x;
		coords.y = mesh->mTextureCoords[0][i].y;

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

	SaveMesh(mesh->mName.C_Str(), vertices, indices, norms, texCoords);

	if (mesh->mMaterialIndex >= 0)
	{
		DEBUG_LOG("Processing material...");
		
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		TextureLoader::GetInstance()->ImportTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		
		DEBUG_LOG("Material loading completed!");
	}
}

MaterialComponent* LoadModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName)
{
	MaterialComponent* material = nullptr;
	// Carefull with multiple textures as it affects imgui
	for (unsigned int i = 0; i < 1; ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string aux = str.C_Str();
		aux = aux.substr(aux.find_last_of("\\") + 1, aux.length());
		std::string path = ASSETS_FOLDER;
		path += aux;
		material = TextureLoader::GetInstance()->LoadTexture(path);
	}

	return material;
}

Uint64 LoadModel::SaveMesh(const char* name, std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float3>& normals, std::vector<float2>& texCoords)
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

	std::string meshName = MESHES_FOLDER;
	meshName += name;
	meshName += ".rgmesh";
	
	if (app->fs->Save(meshName.c_str(), buffer, size) > 0)
		DEBUG_LOG("Mesh %s saved succesfully", meshName);

	return size;
}

void LoadModel::LoadMesh(const char* name, MeshComponent* mesh)
{
	char* buffer = nullptr;

	std::string meshPath = MESHES_FOLDER;
	meshPath += name;
	meshPath += ".rgmesh";
	
	if (app->fs->Load(meshPath.c_str(), &buffer) > 0)
	{
		std::vector<float3> vertices;
		std::vector<unsigned int> indices;
		std::vector<float3> normals;
		std::vector<float2> texCoords;

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
		
		mesh->SetMesh(vertices, indices, texCoords, normals);
	}
	else
		DEBUG_LOG("Mesh file not found!");
}

void LoadModel::LoadingTransform(aiNode* node, GameObject* obj)
{
	aiVector3D pos;
	aiQuaternion quat;
	aiVector3D scale;

	node->mTransformation.Decompose(scale, quat, pos);

	float3 p = { pos.x, pos.y, pos.z };
	Quat q = { quat.x, quat.y, quat.z, quat.w };
	float3 s = { scale.x, scale.y, scale.z };

	obj->GetComponent<TransformComponent>()->SetTransform(p, q, s);
}