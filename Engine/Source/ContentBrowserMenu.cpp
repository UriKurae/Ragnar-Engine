#include "ContentBrowserMenu.h"

#include "Application.h"
#include "FileSystem.h"

#include <filesystem>

#include "Imgui/imgui.h"

ContentBrowserMenu::ContentBrowserMenu() : Menu(true)
{
	currentDirectory = "Assets/";
}

ContentBrowserMenu::~ContentBrowserMenu()
{
}

bool ContentBrowserMenu::Update(float dt)
{
	ImGui::Begin("Content Browser", &active);

	std::vector<std::string> files;
	std::vector<std::string> dirs;

	app->fs->DiscoverFiles(currentDirectory.c_str(), files, dirs);

	float padding = 10.0f;
	float cell = 128 + padding;

	float width = ImGui::GetContentRegionAvail().x;
	int columns = (int)(width / cell);
	if (columns < 1)
		columns = 1;
	ImGui::Columns(columns, 0, false);

	for (std::vector<std::string>::const_iterator it = dirs.begin(); it != dirs.end(); ++it)
	{
		ImGui::Button((*it).c_str(), { 128, 128 });
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (app->fs->IsDirectory((*it).c_str()))
				currentDirectory = (*it);
		}
		
		std::string item = (*it).substr(0, (*it).find_last_of("/"));
		item = item.substr(item.find_last_of("/") + 1, item.length());
		ImGui::Text(item.c_str());

		ImGui::NextColumn();
	}

	for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
	{
		ImGui::ImageButton("", { 128, 128 });
		std::string item = (*it).substr((*it).find_last_of("/") + 1, (*it).length());
		//item = item.substr(0, item.find_last_of("/"));
		ImGui::Text(item.c_str());

		ImGui::NextColumn();
	}

	ImGui::Columns(1);

	ImGui::End();

	return true;
}