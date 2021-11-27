#pragma once

#include "Menu.h"

#include <string>
#include <vector>
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

	std::shared_ptr<Texture> dirIcon;
};