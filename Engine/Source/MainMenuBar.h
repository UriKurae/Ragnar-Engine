#pragma once
#include "Menu.h"
#include <vector>
#include <string>

class ConsoleMenu;
class TextEditorMenu;
class Texture;

enum class Menus
{
	CONSOLE = 0,
	CONFIGURATION = 1,
	ABOUT = 2,
	INSPECTOR = 3,
	HIERARCHY = 4,
	CONTENT_BROWSER = 5,
	TEXT_EDITOR = 6,
	FOGWAR = 7,
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

	//void StyleTheme();
	void AlignWithView();
	void AlignViewWithSelected();

private:
	std::string GetNotLightSensibleShaderSource();
	std::string GetLightSensibleShaderSource();

	void ShowCreateLigthSensibleShaderWindow();
	void ShowCreateNotLigthSensibleShaderWindow();

private:
	bool showMenu;
	Texture* buttonPlay;
	Texture* buttonStop;
	Texture* buttonNextFrame;
	Texture* buttonPause;
	Texture* buttonPauseBlue;

	bool saveWindow;
	std::vector<Menu*> menus;


	bool showCreateLightSensibleShaderWindow = false;
	bool showCreateNotLightSensibleShaderWindow = false;

	std::vector<std::string> stylesList;
	int style = 5;
	float alphaStyle = 0.1f;
};