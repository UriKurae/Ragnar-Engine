#pragma once

#include <vector>
#include <string>

class Texture;
class Mesh;

class ResourceManager
{
public:
	static ResourceManager* GetInstance();
	static void ReleaseInstance();
	~ResourceManager();

	void AddTexture(Texture* tex);
	Texture* IsTextureLoaded(std::string path);
	void RemoveTexture(Texture* tex);

	void AddMesh(Mesh* mesh);
	Mesh* IsMeshLoaded(std::string path);
	void RemoveMesh(Mesh* mesh);

private:
	ResourceManager();

	static ResourceManager* instance;

	std::vector<Texture*> textures;
	std::vector<Mesh*> meshes;
};