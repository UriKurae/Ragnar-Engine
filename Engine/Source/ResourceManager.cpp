#include "ResourceManager.h"

#include "Application.h"
#include "FileSystem.h"
#include "Globals.h"
#include "TextureImporter.h"
#include "MeshImporter.h"
#include "ModelImporter.h"

#include "MathGeoLib/src/Algorithm/Random/LCG.h"

#include "Texture.h"
#include "Mesh.h"
#include "Model.h"

#include <stack>

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

void ResourceManager::CheckForNewResources()
{

}

uint ResourceManager::CreateResource(ResourceType type, std::string& assets, std::string& library)
{
	std::shared_ptr<Resource> resource = nullptr;

	LCG random;
	uint uid = random.IntFast();

	switch (type)
	{
	case ResourceType::NONE:
		break;
	case ResourceType::TEXTURE:
		library = TEXTURES_FOLDER + std::string("texture_") + std::to_string(uid) + ".dds";
		resource = std::make_shared<Texture>(uid, assets, library);
		break;
	case ResourceType::MESH:
		library = MESHES_FOLDER + std::string("mesh_") + std::to_string(uid) + ".rgmesh";
		resource = std::make_shared<Mesh>(uid, assets, library);
		break;
	case ResourceType::MODEL:
		library = MODELS_FOLDER + std::string("model_") + std::to_string(uid) + ".rgmodel";
		resource = std::make_shared<Model>(uid, assets, library);
		break;
	}

	if (resource != nullptr) map[uid] = resource;

	return uid;
}

void ResourceManager::CreateResourceCreated(ResourceType type, std::string& assets, std::string& library)
{
	
}

std::shared_ptr<Resource> ResourceManager::LoadResource(uint uid)
{
	std::shared_ptr<Resource> res = map[uid];
	
	if (res != nullptr) res->Load();

	return res;
}

std::shared_ptr<Resource> ResourceManager::LoadResource(std::string& path)
{
	std::map<uint, std::shared_ptr<Resource>>::iterator it;
	for (it = map.begin(); it != map.end(); ++it)
	{
		std::shared_ptr<Resource> res = (*it).second;
		if (res->GetAssetsPath() == path)
		{
			res->Load();
			return res;
		}
	}
}

bool ResourceManager::CheckResource(std::string& path)
{
	std::map<uint, std::shared_ptr<Resource>>::iterator it;
	for (it = map.begin(); it != map.end(); ++it)
	{
		if ((*it).second->GetAssetsPath() == path)
			return true;
	}
	return false;
}

void ResourceManager::ImportResourcesFromLibrary()
{
	std::vector<std::string> dirs;
	std::vector<std::string> files;

	std::stack<std::string> directories;

	app->fs->DiscoverFilesAndDirs("Library/", files, dirs);

	for (int i = 0; i < dirs.size(); ++i)
	{
		directories.push(dirs[i]);
	}

	dirs.clear();
	
	while (!directories.empty())
	{
		std::string dir = directories.top();

		for (int i = 0; i < files.size(); ++i)
		{
			std::string extension = files[i].substr(files[i].find_last_of("."), files[i].length());

			// TODO: Find a solution to get the assets path
			if (extension.data() == ".rgmodel") CreateResourceCreated(ResourceType::MODEL, std::string(""), files[i]);
			else if (extension.data() == ".rgtexture") CreateResourceCreated(ResourceType::TEXTURE, std::string(""), files[i]);
			else if (extension.data() == ".rgmesh") CreateResourceCreated(ResourceType::MESH, std::string(""), files[i]);
		}
	}
}

void ResourceManager::ImportAllResources()
{
	std::stack<std::string> dirsStack;
	dirsStack.push("Assets/");

	while (!dirsStack.empty())
	{
		// First we get the item at the top of the stack
		std::string dir = dirsStack.top();

		// Then we create the vectors of files and dirs
		std::vector<std::string> files;
		std::vector<std::string> dirs;

		// We get the files and dirs of the current directory, that it's the top of the stack
		app->fs->DiscoverFilesAndDirs(dir.c_str(), files, dirs);

		// We iterate all the files on the current directory
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			ResourceType type = app->fs->CheckExtension(*it);
			switch (type)
			{
			case ResourceType::MODEL:
				ModelImporter::ImportModel(*it);
				break;
			case ResourceType::TEXTURE:
				ModelImporter::ImportModel(*it);
				break;
			}
		}

		// Now we pop the current directory we don't need it anymore
		dirsStack.pop();

		// Finally we iterate all the dirs on the current directory and we push them to the stack
		for (std::vector<std::string>::iterator it = dirs.begin(); it != dirs.end(); ++it)
		{
			dirsStack.push(*it);
		}
	}
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
	
	return /*TextureImporter::LoadTexture(p.c_str())*/ nullptr;
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
	return /*MeshImporter::LoadMesh(p.c_str())*/ nullptr;
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