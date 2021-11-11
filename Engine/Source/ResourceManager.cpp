#include "ResourceManager.h"

#include "Application.h"
#include "FileSystem.h"
#include "Globals.h"

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

void ResourceManager::AddTexture(Texture* tex)
{
	textures.emplace_back(tex);
}

Texture* ResourceManager::IsTextureLoaded(std::string path)
{
	if (path.find(".dds") == std::string::npos)
	{
		path = path.substr(path.find_last_of("/") + 1, path.length());
		path = path.substr(0, path.find_last_of("."));
		path = MATERIALS_FOLDER + path + ".dds";
	}

	for (int i = 0; i < textures.size(); ++i)
	{
		if (textures[i]->GetPath() == path)
			return textures[i];
	}
	return nullptr;
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

Mesh* ResourceManager::IsMeshLoaded(std::string& path)
{
	for (int i = 0; i < meshes.size(); ++i)
	{
		if (meshes[i]->GetPath() == path)
			return meshes[i];
	}
	return nullptr;
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