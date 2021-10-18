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

private:
	std::vector<GameObject*> gameObjects;
};