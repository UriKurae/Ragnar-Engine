#include "ContentBrowserMenu.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleSceneManager.h"
#include "MonoManager.h"

#include "FileSystem.h"
#include "ResourceManager.h"
#include "Texture.h"

#include "Profiling.h"

ContentBrowserMenu::ContentBrowserMenu() : sceneIcon(nullptr), dirIcon(nullptr), modelIcon(nullptr), picIcon(nullptr), refreshTime(0.0f), Menu(true, "Content Browser")
{
	mainDirectory = "Assets/";
	currentDirectory = mainDirectory;
}

ContentBrowserMenu::~ContentBrowserMenu()
{
	RELEASE(dirIcon);
	RELEASE(picIcon);
	RELEASE(modelIcon);
	RELEASE(sceneIcon);
	RELEASE(prefabIcon);
	RELEASE(scriptIcon);
}

bool ContentBrowserMenu::Start()
{
	dirIcon = new Texture(-1, std::string("Settings/EngineResources/folder.rgtexture"));
	dirIcon->Load();

	picIcon = new Texture(-2, std::string("Settings/EngineResources/picture.rgtexture"));
	picIcon->Load();

	modelIcon = new Texture(-3, std::string("Settings/EngineResources/model.rgtexture"));
	modelIcon->Load();

	sceneIcon = new Texture(-4, std::string("Settings/EngineResources/logo.rgtexture"));
	sceneIcon->Load();

	prefabIcon = new Texture(-5, std::string("Settings/EngineResources/prefab.rgtexture"));
	prefabIcon->Load();

	scriptIcon = new Texture(-6, std::string("Settings/EngineResources/script.rgtexture"));
	scriptIcon->Load();

	return true;
}

bool ContentBrowserMenu::Update(float dt)
{
	RG_PROFILING_FUNCTION("Content Browser Update");

	std::vector<std::string> files;
	std::vector<std::string> dirs;

	if (app->sceneManager->GetGameState() == GameState::NOT_PLAYING)
	{
		refreshTime += dt;
		if (refreshTime >= 5.0f)
		{
			ResourceManager::GetInstance()->ImportAllResources();
			refreshTime = 0.0f;
		}
	}
	
	app->fs->DiscoverFilesAndDirs("Assets/", files, dirs);
	dirs.erase(std::find(dirs.begin(), dirs.end(), "Assets/ParticlesTemplates/"));
	
	ImGui::Begin(ICON_FA_FOLDER" Content Browser", &active);
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 150);

	ImGuiTreeNodeFlags flags = (currentDirectory == mainDirectory ? ImGuiTreeNodeFlags_Selected : 0) | SetFlags(dirs);
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
	float cell = 100;
	float height = 64;

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
		if ((*it).find("Assets/ParticlesTemplates/") == std::string::npos)
		{
			std::string item = (*it);
			app->fs->GetRelativeDirectory(item);

			bool selected = false;
			if (currentFile == *it)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.29f, 0.66f, 1.0f)));
				selected = true;
			}
			ImGui::ImageButton(dirIcon ? (ImTextureID)dirIcon->GetId() : 0, { cell, height });
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

			if (selected) ImGui::PopStyleColor();

			ImGui::NextColumn();
		}
	}

	if (!ImGui::IsWindowFocused()) currentFile = "";

	int i = 0;
	for (std::vector<std::string>::const_iterator it = files2.begin(); it != files2.end(); ++it)
	{
		ImGui::PushID(i++);
		std::string item = (*it);
		
		ResourceType type = app->fs->CheckExtension(item);
		app->fs->GetFilenameWithExtension(item);
		
		bool selected = false;
		if (currentFile == *it)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.29f, 0.66f, 1.0f)));
			selected = true;
		}

		switch (type)
		{
		case ResourceType::TEXTURE:
			ImGui::ImageButton(picIcon ? (ImTextureID)picIcon->GetId() : "", { cell, height });
			break;
		case ResourceType::MODEL:
			ImGui::ImageButton(modelIcon ? (ImTextureID)modelIcon->GetId() : "", { cell, height });
			break;
		case ResourceType::SCENE:
			ImGui::ImageButton(sceneIcon ? (ImTextureID)sceneIcon->GetId() : "", { cell, height });
			break;
		case ResourceType::SCRIPT:
			ImGui::ImageButton(scriptIcon ? (ImTextureID)scriptIcon->GetId() : "", { cell, height });
			break;
		default:
			if ((*it).find(".rgprefab") != std::string::npos)
			{
				ImGui::ImageButton(prefabIcon ? (ImTextureID)prefabIcon->GetId() : "", { cell, cell });
			}
			else
			{
				ImGui::Button(item.c_str());
			}
			break;
		}
		if (ImGui::IsItemClicked())
		{
			app->editor->SetResource(ResourceManager::GetInstance()->GetResource((*it)).get());
			currentFile = (*it);
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
			{
				ResourceType type = app->fs->CheckExtension(*it);
				if (type == ResourceType::SCRIPT)
				{
					ShellExecute(0, 0, "Assembly-CSharp.sln", 0, 0, SW_SHOW);
				}
			}
		}
		if (ImGui::BeginDragDropSource())
		{
			const wchar_t* path = (const wchar_t*)(*it).c_str();
			ImGui::SetDragDropPayload("Content Browser", path, (wcslen(path) + 1) * sizeof(wchar_t), ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}
		ImGui::Text(item.c_str());
		if (selected) ImGui::PopStyleColor();

		ImGui::NextColumn();

		ImGui::PopID();
	}

	if (app->input->GetKey(SDL_SCANCODE_DELETE) == KeyState::KEY_UP)
	{
		if (!currentFile.empty())
		{
			ResourceManager::GetInstance()->DeleteResource(currentFile);
			app->editor->SetResource(nullptr);
			currentFile.clear();
		}
	}

	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::BeginMenu("Create C# Script"))
		{
			DrawCreationPopup("Script name: ", ".cs", std::bind(&MonoManager::CreateAssetsScript, app->moduleMono, std::placeholders::_1));
			std::bind(&MonoManager::UpdateListScripts, app->moduleMono, std::placeholders::_1);
			ImGui::EndMenu();
		}
		ImGui::EndPopup();
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

		ImGuiTreeNodeFlags flags = (currentDirectory == (*it) ? ImGuiTreeNodeFlags_Selected : 0) | SetFlags(dirs);

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

void ContentBrowserMenu::DrawCreationPopup(const char* popDisplay, const char* dotExtension, std::function<void(const char*)> f)
{
	static char name[50] = "\0";

	ImGui::Text(popDisplay); ImGui::SameLine();

	std::string id("##");
	id += dotExtension;

	ImGui::InputText(id.c_str(), name, sizeof(char) * 50);
	if (ImGui::Button("Create"))
	{
		std::string path = name;
		if (path.find('.') == path.npos)
			path += dotExtension;

		//TODO: Check if the extension is correct, to avoid a .cs.glsl file
		if (path.find(dotExtension) != path.npos)
		{
			f(path.c_str());
			name[0] = '\0';
		}

		ImGui::CloseCurrentPopup();
	}
}

ImGuiTreeNodeFlags ContentBrowserMenu::SetFlags(std::vector<std::string> node)
{
	// This flags allow to open the tree if you click on arrow or doubleClick on object, by default the tree is open  
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	// If GameObject doesn't childrens = no collapsing and no arrow
	//if (node.size() == 0)
	//	flags |= ImGuiTreeNodeFlags_Leaf;

	return flags;
}