#pragma once

#include <string>

class GameObject;

class PrefabManager
{
public:
	static PrefabManager* GetInstance();
	static void ReleaseInstance();
	~PrefabManager();

	void SavePrefab(GameObject* gameObject, int option);
	void LoadPrefab(const char* path);
	void UpdatePrefabs(GameObject* prefabPath);

private:
	PrefabManager();

	std::string RenameFile(GameObject* gameObject, std::string& fileName);

	static PrefabManager* instance;
};