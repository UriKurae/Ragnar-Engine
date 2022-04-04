#include "ResourceManager.h"
#include "Application.h"
#include "Globals.h"

#include "FileSystem.h"
#include "TextureImporter.h"
#include "ModelImporter.h"
#include "ShaderImporter.h"
#include "AnimationImporter.h"

#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "Scene.h"

#include <stack>
#include <fstream>
#include "Algorithm/Random/LCG.h"

#include "Profiling.h"

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
	/*for (int i = 0; i < textures.size(); ++i)
	{
		RELEASE(textures[i]);
	}
	textures.clear();

	for (int i = 0; i < meshes.size(); ++i)
	{
		RELEASE(meshes[i]);
	}
	meshes.clear();*/
}

void ResourceManager::CheckForNewResources()
{

}

uint ResourceManager::CreateResource(ResourceType type, std::string assets, std::string& library)
{
	std::shared_ptr<Resource> resource = nullptr;

	std::unordered_map<uint, std::shared_ptr<Resource>>::iterator it;
	
	for (it = map.begin(); it != map.end(); ++it)
	{
		if ((*it).second->GetAssetsPath() == assets)
		{
			library = (*it).second->GetLibraryPath();
			return (*it).first;
		}
	}

	LCG random;
	uint uid = random.IntFast();

	switch (type)
	{
	case ResourceType::NONE:
		break;
	case ResourceType::SCENE:
		library = SCENES_LIBRARY_FOLDER + std::string("scenes_") + std::to_string(uid) + ".ragnar";
		resource = std::make_shared<Scene>(uid, assets, library);
		break;
	case ResourceType::TEXTURE:
		library = TEXTURES_FOLDER + std::string("texture_") + std::to_string(uid) + ".rgtexture";
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
	case ResourceType::ANIMATION:
		library = ANIMATIONS_FOLDER + std::string("anim_") + std::to_string(uid) + ".rganim";
		resource = std::make_shared<Animation>(uid, assets, library);
		break;
	case ResourceType::BONE:
		library = BONES_FOLDER + std::string("bone_") + std::to_string(uid) + ".rgbone";
		resource = std::make_shared<Bone2>(uid, assets, library);
		break;
	case ResourceType::SHADER:
		library = SHADERS_FOLDER + std::string("shader_") + std::to_string(uid) + ".shader";
		resource = std::make_shared<Shader>(uid, assets, library);
		
		std::ifstream src(assets, std::ios::binary);
		std::ofstream dest(library, std::ios::binary);
		dest << src.rdbuf();

		break;
	}

	if (resource != nullptr) map[uid] = resource;

	return uid;
}

void ResourceManager::CreateResourceCreated(ResourceType type, uint uid, std::string& assets, std::string& library)
{
	std::shared_ptr<Resource> resource = nullptr;
	switch (type)
	{
	case ResourceType::TEXTURE:
		resource = std::make_shared<Texture>(uid, assets, library);
		break;
	case ResourceType::MESH:
		resource = std::make_shared<Mesh>(uid, assets, library);
		break;
	case ResourceType::MODEL:
		resource = std::make_shared<Model>(uid, assets, library);
		break;
	case ResourceType::ANIMATION:
		resource = std::make_shared<Animation>(uid, assets, library);
		break;
	case ResourceType::BONE:
		resource = std::make_shared<Bone2>(uid, assets, library);
		break;
	case ResourceType::SHADER:
		resource = std::make_shared<Shader>(uid, assets, library);
		break;
	case ResourceType::SCENE:
		resource = std::make_shared<Scene>(uid, assets, library);
		break;
	default:
		break;
	}

	if (resource != nullptr && resource)
		map[uid] = resource;
}

std::shared_ptr<Resource> ResourceManager::LoadResource(uint uid)
{
	std::shared_ptr<Resource> res = nullptr;
	std::unordered_map<uint, std::shared_ptr<Resource>>::iterator it;
	it = map.find(uid);
	if (it != map.end())
	{
		res = map[uid];
		if (res != nullptr) res->Load();
	}

	return res;
}

std::shared_ptr<Resource> ResourceManager::LoadResource(std::string& path)
{
	std::unordered_map<uint, std::shared_ptr<Resource>>::iterator it;
	for (it = map.begin(); it != map.end(); ++it)
	{
		std::shared_ptr<Resource> res = (*it).second;
		std::string p = res->GetAssetsPath();
		if (res->GetAssetsPath() == path)
		{
			res->Load();
			return res;
		}
	}
}

bool ResourceManager::CheckResource(std::string& path)
{
	std::unordered_map<uint, std::shared_ptr<Resource>>::iterator it;
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

	std::stack<std::string> directories;

	app->fs->DiscoverDirs("Library/", dirs);

	for (int i = 0; i < dirs.size(); ++i)
	{
		directories.push(dirs[i]);
	}

	dirs.clear();
	
	while (!directories.empty())
	{
		std::string dir = directories.top();

		std::vector<std::string> files;
		app->fs->DiscoverFiles(dir.c_str(), files);

		for (int i = 0; i < files.size(); ++i)
		{
			if (files[i].find(".rg") != std::string::npos || files[i].find(".shader") != std::string::npos
				|| files[i].find(".ragnar") != std::string::npos)
			{
				std::string extension = files[i].substr(files[i].find_last_of("."), files[i].length());
				std::string metaFile = dir + files[i].substr(0, files[i].find_last_of(".")) + ".meta";

				char* buffer = nullptr;
				if (app->fs->Load(metaFile.c_str(), &buffer) > 0)
				{
					JsonParsing metaInfo(buffer);

					std::string assets(metaInfo.GetJsonString("Assets Path"));
					uint uid = metaInfo.GetJsonNumber("Uuid");

					// TODO: Find a solution to get the assets path
					if (files[i].find(".rgmodel") != std::string::npos) CreateResourceCreated(ResourceType::MODEL, uid, assets, dir + files[i]);
					else if (files[i].find(".rgtexture") != std::string::npos) CreateResourceCreated(ResourceType::TEXTURE, uid, assets, dir + files[i]);
					else if (files[i].find(".rgmesh") != std::string::npos) CreateResourceCreated(ResourceType::MESH, uid, assets, dir + files[i]);
					else if (files[i].find(".rganim") != std::string::npos) CreateResourceCreated(ResourceType::ANIMATION, uid, assets, dir + files[i]);
					else if (files[i].find(".rgbone") != std::string::npos) CreateResourceCreated(ResourceType::BONE, uid, assets, dir + files[i]);
					else if (files[i].find(".shader") != std::string::npos)
						CreateResourceCreated(ResourceType::SHADER, uid, assets, dir + files[i]);
					else if(files[i].find(".ragnar") != std::string::npos)
						CreateResourceCreated(ResourceType::SCENE, uid, assets, dir + files[i]);

					RELEASE_ARRAY(buffer);
				}
			}
		}

		directories.pop();
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
				TextureImporter::ImportTexture(*it);
				break;
			case ResourceType::SHADER:
				ShaderImporter::SaveShader(*it);
				//ResourceManager::GetInstance()->CreateResource(ResourceType::SHADER, std::string("Assets/Resources/Shaders/default.shader"), std::string());
				break;
			case ResourceType::SCENE:
				uint uuid = CreateResource(ResourceType::SCENE, *it, *it);
				std::shared_ptr<Scene> scene = std::static_pointer_cast<Scene>(GetResource(uuid));

				// We dont really need to encrypt the data, so the file gets copied
				CopyFileA(scene->GetAssetsPath().c_str(), scene->GetLibraryPath().c_str(), true);
				
				JsonParsing metaFile;

				metaFile.SetNewJsonString(metaFile.ValueToObject(metaFile.GetRootValue()), "Assets Path", scene->GetAssetsPath().c_str());
				metaFile.SetNewJsonNumber(metaFile.ValueToObject(metaFile.GetRootValue()), "Uuid", uuid);

				char* buffer = nullptr;
				size_t size = metaFile.Save(&buffer);
				
				std::string p = scene->GetLibraryPath().substr(0, scene->GetLibraryPath().find("."));
				p += ".meta";
				app->fs->Save(p.c_str(), buffer, size);

				RELEASE_ARRAY(buffer);
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

std::shared_ptr<Resource> ResourceManager::GetResource(uint uid)
{
	return map[uid];
}

std::shared_ptr<Resource> ResourceManager::GetResource(std::string path)
{
	std::unordered_map<uint, std::shared_ptr<Resource>>::iterator it = map.begin();

	for (; it != map.end(); ++it)
	{
		if ((*it).second->GetAssetsPath() == path)
			return (*it).second;
	}

	return nullptr;
}

void ResourceManager::DeleteResource(std::string& path)
{
	std::unordered_map<uint, std::shared_ptr<Resource>>::iterator it;

	for (it = map.begin(); it != map.end(); ++it)
	{
		std::shared_ptr<Resource> resource = (*it).second;
		if (resource->GetAssetsPath() == path)
		{
			app->fs->RemoveFile(path.c_str());
			app->fs->RemoveFile(resource->GetLibraryPath().c_str());
			std::string meta = resource->GetLibraryPath();
			meta = meta.substr(0, meta.find_last_of(".")) + ".meta";
			app->fs->RemoveFile(meta.c_str());
			map.erase(it);
			return;
		}
	}
}

void ResourceManager::DeleteResource(uint uid)
{
	if (map.find(uid) != map.end())
	{
		map.erase(uid);
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

std::vector<std::shared_ptr<Scene>> ResourceManager::GetScenes()
{
	std::vector<std::shared_ptr<Scene>> scenes;
	
	for (std::unordered_map<uint, std::shared_ptr<Resource>>::iterator it = map.begin(); it != map.end(); ++it)
	{
		if ((*it).second->GetType() == ResourceType::SCENE)
		{
			scenes.push_back(std::static_pointer_cast<Scene>((*it).second));
		}
	}

	return scenes;
}