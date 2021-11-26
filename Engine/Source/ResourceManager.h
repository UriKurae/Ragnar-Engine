#pragma once

#include "Globals.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

class Texture;
class Mesh;
class Resource;
enum class ResourceType;

class ResourceManager
{
public:
	static ResourceManager* GetInstance();
	static void ReleaseInstance();
	~ResourceManager();

	uint CreateResource(ResourceType type, std::string& assets, std::string& library);

	std::shared_ptr<Resource> LoadResource(uint uid);

	void AddTexture(Texture* tex);
	Texture* IsTextureLoaded(std::string path);
	void RemoveTexture(Texture* tex);

	void AddMesh(Mesh* mesh);
	Mesh* IsMeshLoaded(std::string path);
	void RemoveMesh(Mesh* mesh);

private:
	ResourceManager();

	static ResourceManager* instance;

	std::map<uint, std::shared_ptr<Resource>> map;
	std::vector<Texture*> textures;
	std::vector<Mesh*> meshes;
};