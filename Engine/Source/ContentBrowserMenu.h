#pragma once

#include "Menu.h"

#include <string>
#include <vector>


class Texture;

class ContentBrowserMenu : public Menu
{
public:
	ContentBrowserMenu();
	~ContentBrowserMenu();

	bool Start() override;

	bool Update(float dt) override;

	void DrawRecursive(std::vector<std::string>& dirs);

	ImGuiTreeNodeFlags SetFlags(std::vector<std::string> node);

	inline std::string& GetCurrentDir() { return currentDirectory; }

private:
	std::string currentDirectory;
	std::string mainDirectory;

	std::string currentFile;

	Texture* dirIcon;
	Texture* picIcon;
	Texture* modelIcon;
	Texture* sceneIcon;
	Texture* prefabIcon;
};