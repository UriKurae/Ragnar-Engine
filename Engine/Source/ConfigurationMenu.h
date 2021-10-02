#pragma once

#include "Menu.h"

#include <vector>

class ConfigurationMenu : public Menu
{
public:

	ConfigurationMenu();
	~ConfigurationMenu();

	bool Update(float dt) override;

private:
	std::vector<float> fps;
	std::vector<float> ms;
	std::vector<float> memory;

	int memoryCount;

	bool activeWindow;
	bool openOptions;
	bool activeInput;
	bool activeHardware;
};