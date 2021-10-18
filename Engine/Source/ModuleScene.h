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

	GameObject* CreateGameObject();

private:
	std::vector<GameObject*> gameObjects;
};