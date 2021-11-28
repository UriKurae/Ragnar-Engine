#include "ContentBrowserMenu.h"

#include "Application.h"
#include "ModuleInput.h"
#include "FileSystem.h"
#include "ResourceManager.h"

#include "Texture.h"
#include "TextureImporter.h"

#include "Imgui/imgui.h"
#include <iostream>
#include <thread>

#include "Profiling.h"

ContentBrowserMenu::ContentBrowserMenu() : Menu(true)
{
	mainDirectory = "Assets/";
	currentDirectory = mainDirectory;
	
	dirIcon = nullptr;
}

ContentBrowserMenu::~ContentBrowserMenu()
{
	RELEASE(dirIcon);
	RELEASE(picIcon);
	RELEASE(modelIcon);
}

bool ContentBrowserMenu::Start()
{
	//TextureImporter::ImportTexture(std::string("Assets/Resources/model.png"));
	dirIcon = new Texture(-1, std::string(""), std::string("Settings/EngineResources/folder.rgtexture"));
	dirIcon->Load();

	picIcon = new Texture(-2, std::string(""), std::string("Settings/EngineResources/pic.rgtexture"));
	picIcon->Load();

	modelIcon = new Texture(-3, std::string(""), std::string("Settings/EngineResources/model.rgtexture"));
	modelIcon->Load();

	return true;
}

static void UpdatingResources()
{
	ResourceManager::GetInstance()->ImportAllResources();
}

bool ContentBrowserMenu::Update(float dt)
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;

	if (resource.joinable()) resource.join();
	resource = std::thread(UpdatingResources);

	app->fs->DiscoverFilesAndDirs("Assets/", files, dirs);
	
	ImGui::Begin("Content Browser", &active);
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 150);

	ImGuiTreeNodeFlags flags = (currentDirectory == mainDirectory ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	bool opened = ImGui::TreeNodeEx("Assets", flags);
	if (ImGui::IsItemClicked())
	{
		currentDirectory = mainDirectory;
	}

	if (opened)
	{
		DrawRecursive(dirs);
		ImGui::TreePop();
	}

	ImGui::NextColumn();

	std::vector<std::string> files2;
	std::vector<std::string> dirs2;

	app->fs->DiscoverFilesAndDirs(currentDirectory.c_str(), files2, dirs2);
	
	ImGui::BeginChild("Assets");

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

	for (std::vector<std::string>::const_iterator it = dirs2.begin(); it != dirs2.end(); ++it)
	{
		std::string item = (*it);
		app->fs->GetRelativeDirectory(item);
		
		ImGui::ImageButton(dirIcon ? (ImTextureID)dirIcon->GetId() : 0, { cell, cell });
		if (ImGui::IsItemClicked())
		{
			currentFile = (*it);
		}
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
	for (std::vector<std::string>::const_iterator it = files2.begin(); it != files2.end(); ++it)
	{
		ImGui::PushID(i++);
		std::string item = (*it);
		
		app->fs->GetFilenameWithExtension(item);
		
		if (item.find(".png") != std::string::npos) ImGui::ImageButton(picIcon ? (ImTextureID)picIcon->GetId() : "", {cell, cell});
		else if (item.find(".fbx") != std::string::npos) ImGui::ImageButton(modelIcon ? (ImTextureID)modelIcon->GetId() : "", { cell, cell });
		else ImGui::ImageButton("", { cell, cell });
		if (ImGui::IsItemClicked())
		{
			currentFile = (*it);
		}
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

	if (app->input->GetKey(SDL_SCANCODE_DELETE) == KeyState::KEY_UP)
	{
		app->fs->RemoveFile(currentFile.c_str());
		currentFile.clear();
	}

	ImGui::Columns(1);

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::EndChild();
	ImGui::End();

	return true;
}

void ContentBrowserMenu::DrawRecursive(std::vector<std::string>& dirs)
{
	for (std::vector<std::string>::iterator it = dirs.begin(); it != dirs.end(); ++it)
	{
		std::string name = (*it);
		app->fs->GetRelativeDirectory(name);

		ImGuiTreeNodeFlags flags = (currentDirectory == (*it) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		
		bool opened = ImGui::TreeNodeEx(name.c_str(), flags);
		if (ImGui::IsItemClicked())
		{
			currentDirectory = (*it);
		}

		if (opened)
		{
			std::vector<std::string> file;
			std::vector<std::string> dir;
			app->fs->DiscoverFilesAndDirs((*it).c_str(), file, dir);

			DrawRecursive(dir);
			ImGui::TreePop();
		}
	}
}