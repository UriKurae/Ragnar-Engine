#pragma once

#include "Module.h"
#include <vector>
#include <string>

class ConsoleMenu;

class ModuleEditor : public Module
{
public:
	ModuleEditor();

	~ModuleEditor();

	bool Update(float dt) override;

	bool SaveConfig(JsonParsing& node) const override;

	void LogConsole(const char* string);

private:
	ConsoleMenu* console;

    bool showMenu;
	bool showHelpMenu;
	bool showConfiguration;
	bool showConsole;

	std::vector<float> fps;
	std::vector<float> ms;

	bool activeWindow;
	bool activeInput;
	bool activeHardware;
};