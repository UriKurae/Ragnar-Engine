#pragma once

#include "Module.h"
#include "MainMenuBar.h"
#include "Viewport.h"

#include <vector>
#include <string>

class GameObject;

class ModuleEditor : public Module
{
public:
	ModuleEditor();

	~ModuleEditor();

	bool Update(float dt) override;
	bool Draw();
	bool CleanUp() override;

	inline GameObject* GetSelected() { return selected; }

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

	void LogConsole(const char* string);

	void ShowChildren(GameObject* parent);

private:
	bool createGameObject;
	bool gameObjectOptions;
	MainMenuBar mainMenuBar;

	GameObject* selected;
	GameObject* selectedParent;

	Viewport* viewport;
};