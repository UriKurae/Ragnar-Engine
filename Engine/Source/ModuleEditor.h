#pragma once

#include "Module.h"
#include <vector>
#include <string>

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* a);

	~ModuleEditor();

	bool Update(float dt) override;

private:
	bool showMenu;
	bool showHelpMenu;

	std::string string;

	std::vector<float> fps;
	std::vector<float> ms;

	std::vector<std::string> strings;

	bool activeWindow;
	bool activeInput;
	bool activeHardware;
};