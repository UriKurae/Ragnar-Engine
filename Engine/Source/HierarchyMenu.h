#pragma once

#include "Menu.h"

class GameObject;

class HierarchyMenu : public Menu
{
public:
	HierarchyMenu();

	~HierarchyMenu();

	bool Update(float dt) override;

	void ShowChildren(GameObject* parent);

private:
	bool gameObjectOptions;

	bool createGameObject;
};