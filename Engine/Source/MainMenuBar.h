#pragma once
#include "Menu.h"
#include <vector>
#include <string>
#include <memory>

class ConsoleMenu;
class TextEditorMenu;
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
	bool showBuildMenu;

	std::shared_ptr<Scene> sceneSelected;

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