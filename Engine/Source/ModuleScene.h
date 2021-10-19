#pragma once

#include "Module.h"
#include <vector>

class GameObject;

class ModuleScene : public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Start() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	GameObject* CreateGameObject();
	inline std::vector<GameObject*> GetGameObjectsList() const 
	{ 
		return gameObjects; 
	}

private:
	std::vector<GameObject*> gameObjects;
};