#pragma once

#include "Menu.h"

#include <string>
#include <vector>
#include <thread>
#include <memory>

class Texture;

class ContentBrowserMenu : public Menu
{
public:
	ContentBrowserMenu();
	~ContentBrowserMenu();

	bool Start() override;

	bool Update(float dt) override;

	void DrawRecursive(std::vector<std::string>& dirs);

	inline std::string& GetCurrentDir() { return currentDirectory; }

private:
	std::string currentDirectory;
	std::string mainDirectory;

	std::string currentFile;

	Texture* dirIcon;
	Texture* picIcon;
	Texture* modelIcon;
	Texture* sceneIcon;
};