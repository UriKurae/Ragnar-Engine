#pragma once
#include <vector>
#include <string>
#include <map>
#include <memory>

class Texture;
class Mesh;
class Resource;
class Scene;
enum class ResourceType;
typedef unsigned int uint;

class ResourceManager
{
public:
	static ResourceManager* GetInstance();
	static void ReleaseInstance();
	~ResourceManager();

	uint CreateResource(ResourceType type, std::string assets, std::string& library);
	void CreateResourceCreated(ResourceType type, uint uid, std::string& assets, std::string& library);

	std::shared_ptr<Resource> LoadResource(uint uid);
	std::shared_ptr<Resource> LoadResource(std::string& path);

	bool CheckResource(std::string& path);
	void ImportResourcesFromLibrary();
	void ImportAllResources();

	std::shared_ptr<Resource> GetResource(uint uid);
	std::shared_ptr<Resource> GetResource(std::string path);

	void DeleteResource(std::string& path);
	void DeleteResource(uint uid);

	std::vector<std::shared_ptr<Scene>> GetScenes();

private:
	ResourceManager();

	static ResourceManager* instance;

	std::map<uint, std::shared_ptr<Resource>> map;
};