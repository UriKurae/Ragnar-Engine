#pragma once

#include "Menu.h"

#include <string>

class Texture;

class ContentBrowserMenu : public Menu
{
public:
	ContentBrowserMenu();
	~ContentBrowserMenu();

	bool Start() override;

	bool Update(float dt) override;

	inline std::string& GetCurrentDir() { return currentDirectory; }

private:
	std::string currentDirectory;

	Texture* dirIcon;
};