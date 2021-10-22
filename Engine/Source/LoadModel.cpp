#include "LoadModel.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Globals.h"
#include "TextureLoader.h"

#include "MathGeoLib/src/MathGeoLib.h"
#include "IL/il.h"
#include "IL/ilut.h"

#include "Optick/include/optick.h"

#include "mmgr/mmgr.h"

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

void LoadModel::LoadingModel(std::string path)
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
	GameObject* object = app->scene->CreateGameObject();
	object->SetName(p.c_str());
	ProcessNode(scene->mRootNode, scene, object);
}

void LoadModel::ProcessNode(aiNode* node, const aiScene* scene, GameObject* obj)
{
	LoadingTransform(node, obj);
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		MeshComponent* m = ProcessMesh(mesh, scene, obj);
		obj->SetName(node->mName.C_Str());
	}

	// Repeat the process until there's no more children
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		if (node->mChildren[i]->mNumMeshes > 0)
		{
			GameObject* object = app->scene->CreateGameObject();
			object->SetParent(obj);
			if (obj != nullptr) obj->AddChild(object);
			ProcessNode(node->mChildren[i], scene, object);
		}
		else
		{
			ProcessNode(node->mChildren[i], scene, obj);
		}
	}
}

MeshComponent* LoadModel::ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* object)
{
	OPTICK_EVENT("Process Mesh");
	std::vector<float3> vertices;
	std::vector<unsigned int> indices;
	std::vector<float2> texCoords;

	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumFaces * 3);
	texCoords.reserve(mesh->mNumVertices);

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		float3 vertex;
		vertex.x = mesh->mVertices[i].x;
		vertex.y = mesh->mVertices[i].y;
		vertex.z = mesh->mVertices[i].z;

		//vertex.normal.x = mesh->mNormals[i].x;
		//vertex.normal.y = mesh->mNormals[i].y;
		//vertex.normal.z = mesh->mNormals[i].z;

		float2 coords;
		coords.x = mesh->mTextureCoords[0][i].x;
		coords.y = mesh->mTextureCoords[0][i].y;

		vertices.push_back(vertex);
		texCoords.push_back(coords);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
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
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		diffuse = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		
		//std::vector<Texture> specular = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		//textures.insert(textures.end(), specular.begin(), specular.end());
	}
	MeshComponent* m = new MeshComponent(vertices, indices, diffuse, texCoords);
	
	m->SetTransform(object->GetComponent<TransformComponent>());
	object->AddComponent(m);
	if (diffuse) object->AddComponent(diffuse);

	return m;
}

MaterialComponent* LoadModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName)
{
	MaterialComponent* material = nullptr;
	// Carefull with multiple textures as it affects imgui
	for (unsigned int i = 0; i < 1; i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path = ASSETS_FOLDER;
		path += str.C_Str();
		material = TextureLoader::GetInstance()->LoadTexture(path);
	}

	return material;
}

void LoadModel::LoadingTransform(aiNode* node, GameObject* obj)
{
	aiVector3D pos;
	aiQuaternion quat;
	aiVector3D scale;

	node->mTransformation.Decompose(scale, quat, pos);

	float3 p = { pos.x, pos.y, pos.z };
	Quat q = { 0, quat.x, quat.y, quat.z };
	float3 s = { scale.x, scale.y, scale.z };

	obj->GetComponent<TransformComponent>()->SetTransform(p, q, s);
}