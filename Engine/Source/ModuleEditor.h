#pragma once

#include "Module.h"
#include <vector>
#include <string>

class ConsoleMenu;

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* a);

	~ModuleEditor();

	bool Update(float dt) override;

public:
	ConsoleMenu* console;

private:

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