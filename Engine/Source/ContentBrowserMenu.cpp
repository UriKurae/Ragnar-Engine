#include "ContentBrowserMenu.h"

#include "Application.h"
#include "FileSystem.h"
#include "ResourceManager.h"

#include "TextureLoader.h"
#include "Texture.h"

#include "Imgui/imgui.h"

ContentBrowserMenu::ContentBrowserMenu() : Menu(true)
{
	currentDirectory = "Assets/";
	dirIcon = nullptr;
}

ContentBrowserMenu::~ContentBrowserMenu()
{
}

bool ContentBrowserMenu::Start()
{
	dirIcon = TextureLoader::GetInstance()->LoadTexture(std::string("Library/Materials/folder.dds"));
	
	return true;
}

bool ContentBrowserMenu::Update(float dt)
{
	ImGui::Begin("Content Browser", &active);

	if (currentDirectory != "Assets/")
	{
		if (ImGui::Button("<-"))
		{
			currentDirectory = currentDirectory.substr(0, currentDirectory.length() - 1);
			currentDirectory = currentDirectory.substr(0, currentDirectory.find_last_of("/") + 1);
		}
	}

	std::vector<std::string> files;
	std::vector<std::string> dirs;

	app->fs->DiscoverFiles(currentDirectory.c_str(), files, dirs);

	float padding = 10.0f;
	float cell = 64 + padding;

	float width = ImGui::GetContentRegionAvail().x;
	int columns = (int)(width / cell);
	if (columns < 1)
		columns = 1;
	ImGui::Columns(columns, 0, false);

	ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
	ImGui::PushStyleColor(ImGuiCol_Border, { 0, 0, 0, 0 });
	ImGui::PushStyleColor(ImGuiCol_BorderShadow, { 0, 0, 0, 0 });

	for (std::vector<std::string>::const_iterator it = dirs.begin(); it != dirs.end(); ++it)
	{
		std::string item = (*it).substr(0, (*it).find_last_of("/"));
		item = item.substr(item.find_last_of("/") + 1, item.length());
		
		ImGui::ImageButton((ImTextureID)dirIcon->GetId(), { cell, cell });
		if (ImGui::BeginDragDropSource())
		{
			const wchar_t* path = (const wchar_t*)(*it).c_str();
			ImGui::SetDragDropPayload("Content Browser", path, (wcslen(path) + 1) * sizeof(wchar_t), ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (app->fs->IsDirectory((*it).c_str()))
				currentDirectory = (*it);
		}
		
		ImGui::Text(item.c_str());

		ImGui::NextColumn();
	}

	int i = 0;
	for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
	{
		ImGui::PushID(i++);
		std::string item = (*it).substr((*it).find_last_of("/") + 1, (*it).length());
		
		Texture* texture = ResourceManager::GetInstance()->IsTextureLoaded(item);
		ImGui::ImageButton(texture ? (ImTextureID)texture->GetId() : "", {cell, cell});

		if (ImGui::BeginDragDropSource())
		{
			const wchar_t* path = (const wchar_t*)(*it).c_str();
			ImGui::SetDragDropPayload("Content Browser", path, (wcslen(path) + 1) * sizeof(wchar_t), ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}
		ImGui::Text(item.c_str());

		ImGui::NextColumn();

		ImGui::PopID();
	}

	ImGui::Columns(1);

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::End();

	return true;
}