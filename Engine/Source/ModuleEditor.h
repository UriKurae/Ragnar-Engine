#pragma once

#include "Module.h"
#include "MainMenuBar.h"

#include <vector>
#include <string>

class ModuleEditor : public Module
{
public:
	ModuleEditor();

	~ModuleEditor();

	bool Update(float dt) override;
	bool Draw();
	bool CleanUp() override;

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

	void LogConsole(const char* string);

private:

	MainMenuBar mainMenuBar;
};