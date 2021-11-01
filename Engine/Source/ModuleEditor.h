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
	bool Draw(Framebuffer* framebuffer);
	bool CleanUp() override;

	inline GameObject* GetSelected() { return selected; }
	inline GameObject* GetSelectedParent() { return selectedParent; }
	
	inline void SetSelected(GameObject* obj) { selected = obj; }
	inline void SetSelectedParent(GameObject* obj) { selectedParent = obj; }

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

	void LogConsole(const char* string);

	inline Viewport* GetViewport() { return viewport; }

private:
	MainMenuBar mainMenuBar;

	GameObject* selected;
	GameObject* selectedParent;

	Viewport* viewport;
};