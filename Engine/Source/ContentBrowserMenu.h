#pragma once

#include "Menu.h"

#include <string>

class ContentBrowserMenu : public Menu
{
public:
	ContentBrowserMenu();
	~ContentBrowserMenu();

	bool Update(float dt) override;

private:
	std::string currentDirectory;
};