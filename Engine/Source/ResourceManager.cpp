#include "ResourceManager.h"

#include "Application.h"
#include "FileSystem.h"
#include "Globals.h"
#include "TextureImporter.h"
#include "MeshImporter.h"

#include "Texture.h"
#include "Mesh.h"

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager* ResourceManager::GetInstance()
{
	if (!instance) instance = new ResourceManager();
	
	return instance;
}

void ResourceManager::ReleaseInstance()
{
	RELEASE(instance);
}

ResourceManager::ResourceManager()
{
	//std::stack<std::string> stack;
	//app->fs->DiscoverFiles();
}

ResourceManager::~ResourceManager()
{
	for (int i = 0; i < textures.size(); ++i)
	{
		RELEASE(textures[i]);
	}
	textures.clear();

	for (int i = 0; i < meshes.size(); ++i)
	{
		RELEASE(meshes[i]);
	}
	meshes.clear();
}

std::shared_ptr<Resource> ResourceManager::CreateResource(ResourceType type, uint uid)
{
	std::shared_ptr<Resource> resource = nullptr;

	switch (type)
	{
	case ResourceType::NONE:
		break;
	case ResourceType::TEXTURE:
		resource = std::make_shared<Texture>(uid);
		break;
	case ResourceType::MESH:
		resource = std::make_shared<Mesh>(uid);
		break;
	case ResourceType::MODEL:
		//resource = std::make_shared<>();
		break;
	}

	if (resource != nullptr) map[uid] = resource;

	return resource;
}

void ResourceManager::AddTexture(Texture* tex)
{
	textures.emplace_back(tex);
}

Texture* ResourceManager::IsTextureLoaded(std::string path)
{
	std::string p = path;
	if (p.find(".dds") == std::string::npos)
	{
		app->fs->GetFilenameWithoutExtension(p);
		p = TEXTURES_FOLDER + p + ".dds";
	}

	for (int i = 0; i < textures.size(); ++i)
	{
		if (textures[i]->GetPath() == p)
			return textures[i];
	}
	
	return TextureImporter::LoadTexture(p.c_str());
}

void ResourceManager::RemoveTexture(Texture* tex)
{
	for (std::vector<Texture*>::const_iterator it = textures.begin(); it != textures.end(); ++it)
	{
		if ((*it) == tex)
		{
			RELEASE(tex);
			textures.erase(it);
			textures.shrink_to_fit();
			break;
		}
	}
}

void ResourceManager::AddMesh(Mesh* mesh)
{
	meshes.emplace_back(mesh);
}

Mesh* ResourceManager::IsMeshLoaded(std::string path)
{
	std::string p = path;
	if (p.find(".rgmesh") == std::string::npos)
	{
		app->fs->GetFilenameWithoutExtension(p);
		p = MESHES_FOLDER + p + ".dds";
	}

	for (int i = 0; i < meshes.size(); ++i)
	{
		if (meshes[i]->GetPath() == p)
			return meshes[i];
	}
	return MeshImporter::LoadMesh(p.c_str());
}

void ResourceManager::RemoveMesh(Mesh* mesh)
{
	for (std::vector<Mesh*>::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		if ((*it) == mesh)
		{
			RELEASE(mesh);
			meshes.erase(it);
			meshes.shrink_to_fit();
			break;
		}
	}
}