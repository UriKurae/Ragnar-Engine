#pragma once

#include "Menu.h"
#include <vector>

class ConsoleMenu;

enum class Menus
{
	CONSOLE = 0,
	CONFIGURATION = 1,
	ABOUT = 2,
	INSPECTOR = 3,
	HIERARCHY = 4
};

class MainMenuBar : public Menu
{
public:
	MainMenuBar();
	~MainMenuBar();

	bool Update(float dt) override;
	bool CleanUp() override;

	ConsoleMenu* GetConsole();

private:
	bool showMenu;

	std::vector<Menu*> menus;
};