#pragma once

#include "Module.h"
#include <vector>

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* a);

	~ModuleEditor();

	bool Update(float dt) override;

private:
	bool showMenu;
	bool showHelpMenu;
	std::vector<float> fps;
	std::vector<float> ms;
};