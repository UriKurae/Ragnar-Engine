#pragma once

#include "Menu.h"

#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <functional>

class Texture;

class ContentBrowserMenu : public Menu
{
public:
	ContentBrowserMenu();
	~ContentBrowserMenu();

	bool Start() override;

	bool Update(float dt) override;

	void DrawRecursive(std::vector<std::string>& dirs);

	void DrawCreationPopup(const char* popName, const char* dotExtension, std::function<void(const char*)> f);

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