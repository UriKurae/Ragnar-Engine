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

private:
	std::string currentDirectory;

	Texture* dirIcon;
};