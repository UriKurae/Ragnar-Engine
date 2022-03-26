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
	NAVIGATOR,
	INSPECTOR
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
	std::string GetNotLightSensibleShaderSource();
	std::string GetLightSensibleShaderSource();

	void ShowCreateLigthSensibleShaderWindow();
	void ShowCreateNotLigthSensibleShaderWindow();

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
	bool showMenu = false;
	bool showCreateLightSensibleShaderWindow = false;
	bool showCreateNotLightSensibleShaderWindow = false;

	std::vector<Menu*> menus;
	std::vector<std::string> stylesList;
	std::vector<std::string> iconList;

	int style = 5;
	float alphaStyle = 0.1f;
};