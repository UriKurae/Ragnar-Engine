#pragma once

#include "Menu.h"
#include <vector>
#include <string>

class ConsoleMenu;
class Texture;
enum class Menus
{
	CONSOLE = 0,
	CONFIGURATION = 1,
	ABOUT = 2,
	INSPECTOR = 3,
	HIERARCHY = 4,
	CONTENT_BROWSER = 5,
};

class MainMenuBar : public Menu
{
public:
	MainMenuBar();
	~MainMenuBar();

	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	ConsoleMenu* GetConsole();
	std::string& GetCurrentDir();

	void StyleTheme();

private:
	bool showMenu;
	Texture* buttonPlay;
	Texture* buttonStop;
	Texture* buttonNextFrame;
	Texture* buttonPause;
	Texture* buttonPauseBlue;

	bool saveWindow;
	std::vector<Menu*> menus;
};