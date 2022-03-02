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

	ImGuiTreeNodeFlags SetFlags(GameObject* node);

private:
	bool gameObjectOptions;
};