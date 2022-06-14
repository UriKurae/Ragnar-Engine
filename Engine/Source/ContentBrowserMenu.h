#pragma once
#include "Menu.h"

#include <vector>
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
	ImGuiTreeNodeFlags SetFlags(std::vector<std::string> node);

	inline std::string& GetCurrentDir() { return currentDirectory; }

private:
	float refreshTime;

	std::string currentDirectory;
	std::string mainDirectory;

	std::string currentFile;

	Texture* dirIcon;
	Texture* picIcon;
	Texture* modelIcon;
	Texture* sceneIcon;
	Texture* prefabIcon;
	Texture* scriptIcon;
};