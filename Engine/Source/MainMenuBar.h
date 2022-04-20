#pragma once
#include "Menu.h"
#include <vector>
#include <memory>

class ConsoleMenu;
class Texture;
class Scene;

enum class Menus
{
	CONSOLE,
	CONFIGURATION,
	ABOUT,
	HIERARCHY,
	CONTENT_BROWSER,
	TEXT_EDITOR,
	FOGWAR,
	INPUT_ACTION,
	NAVIGATOR,
	INSPECTOR,
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
	std::vector<Menu*> GetMenus() { return menus; };

	int GetStyle() { return style; };
	void SetStyle(int _style);

private:

	//Menus
	bool FileMenu();
	void EditMenu();
	void WindowMenu();
	void ViewMenu();
	void GameObjectMenu();
	void CreateGameObjectMenu();
	void HelpMenu();
	void SetStyleMenu();

	void PlayBar();

private:
	std::vector<Menu*> menus;
	std::vector<std::string> stylesList;
	std::vector<std::string> iconList;

	bool showMenu = false;
	int style = 5;
	float alphaStyle = 0.1f;
};