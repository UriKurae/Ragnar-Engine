#include "Application.h"
#include "Globals.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"

#include "ConsoleMenu.h"
#include "ConfigurationMenu.h"
#include "MainMenuBar.h"
#include "AboutMenu.h"
#include "InspectorMenu.h"
#include "HierarchyMenu.h"
#include "ContentBrowserMenu.h"
#include "Texture.h"
#include "TextureImporter.h"
#include "ResourceManager.h"

#include "Profiling.h"

MainMenuBar::MainMenuBar() : Menu(true), buttonPlay(nullptr), buttonStop(nullptr), buttonNextFrame(nullptr)
{
	showMenu = false;

	menus.reserve(6);
	menus.emplace_back(new ConsoleMenu());
	menus.emplace_back(new ConfigurationMenu());
	menus.emplace_back(new AboutMenu());
	menus.emplace_back(new InspectorMenu());
	menus.emplace_back(new HierarchyMenu());
	menus.emplace_back(new ContentBrowserMenu());
}

MainMenuBar::~MainMenuBar()
{
}

bool MainMenuBar::Start()
{
	//TextureImporter::ImportTexture(std::string("Assets/Resources/PlayButton.png"));
	//TextureImporter::ImportTexture(std::string("Assets/Resources/PauseButton.png"));
	//TextureImporter::ImportTexture(std::string("Assets/Resources/NextFrame.png"));

	buttonPlay = new Texture(-4, std::string(""), std::string("Settings/EngineResources/PlayButton.rgtexture"));
	buttonPlay->Load();

	buttonStop = new Texture(-5, std::string(""), std::string("Settings/EngineResources/PauseButton.rgtexture"));
	buttonStop->Load();

	buttonNextFrame = new Texture(-6, std::string(""), std::string("Settings/EngineResources/NextFrame.rgtexture"));
	buttonNextFrame->Load();

	for (int i = 0; i < menus.size(); ++i)
	{
		menus[i]->Start();
	}

	return true;
}

bool MainMenuBar::Update(float dt)
{
//	ImGui::DockSpaceOverViewport();

	if (ImGui::BeginMainMenuBar())
	{
		bool ret = false;

		if (ImGui::BeginMenu("File"))
		{
			// Project options (Create, open...)
			ImGui::MenuItem("New Project", NULL, &ret);
			ImGui::MenuItem("Open Project", NULL, &ret);

			ImGui::Separator();

			if (ImGui::MenuItem("Load", "Ctrl + L", &ret))
			{
				app->scene->LoadScene("Assets/Scenes/scene.json");
			}
			if (ImGui::MenuItem("Save", "Ctrl + S", &ret))
			{
				app->scene->SaveScene();
			}
			if (ImGui::MenuItem("Exit", "ESC", &ret))
			{
				return false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the file menu");
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("Undo", "Ctrl + Z", &ret);
			ImGui::MenuItem("Redo", "Ctrl + Y", &ret);
			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the edit menu");
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("Console", NULL, &GetConsole()->active);
			ImGui::MenuItem("Configuration", NULL, &menus[(int)Menus::CONFIGURATION]->active);
			ImGui::MenuItem("Hierarchy", NULL, &menus[(int)Menus::HIERARCHY]->active);
			ImGui::MenuItem("Inspector", NULL, &menus[(int)Menus::INSPECTOR]->active);

			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the window menu");
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Depth Test", NULL, app->renderer3D->GetDepthTest()))
			{
				app->renderer3D->SetDepthTest();
			}
			if (ImGui::MenuItem("Cull Face", NULL, app->renderer3D->GetCullFace()))
			{
				app->renderer3D->SetCullFace();
			}
			if (ImGui::MenuItem("Lighting", NULL, app->renderer3D->GetLighting()))
			{
				app->renderer3D->SetLighting();
			}
			if (ImGui::MenuItem("Color Material", NULL, app->renderer3D->GetColorMaterial()))
			{
				app->renderer3D->SetColorMaterial();
			}
			if (ImGui::MenuItem("Texture 2D", NULL, app->renderer3D->GetTexture2D()))
			{
				app->renderer3D->SetTexture2D();
			}
			if (ImGui::MenuItem("Stencil", NULL, app->renderer3D->GetStencil()))
			{
				app->renderer3D->SetStencil();
			}
			if (ImGui::MenuItem("Blending", NULL, app->renderer3D->GetBlending()))
			{
				app->renderer3D->SetBlending();
			}
			if (ImGui::MenuItem("Wire", NULL, app->renderer3D->GetWireMode()))
			{
				app->renderer3D->SetWireMode();
			}
			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the view menu");
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("Demo Menu", NULL, &showMenu);
			ImGui::MenuItem("About Ragnar Engine", "", &menus[(int)Menus::ABOUT]->active);
			if (ImGui::MenuItem("Documentation", "F1", &ret))
			{
				app->RequestBrowser("https://github.com/UriKurae/Ragnar-Engine");
			}
			if (ImGui::MenuItem("Report a Bug", "", &ret))
			{
				app->RequestBrowser("https://github.com/UriKurae/Ragnar-Engine/issues");
			}
			if (ImGui::MenuItem("Download latest", "", &ret))
			{
				app->RequestBrowser("https://github.com/UriKurae/Ragnar-Engine/releases");
			}
			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the help menu");
		}
		ImGui::EndMainMenuBar();
	}

	bool ret = true;
	ImGui::Begin(" ", &ret, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
	ImGui::PushStyleColor(ImGuiCol_Border, { 0, 0, 0, 0 });
	ImGui::PushStyleColor(ImGuiCol_BorderShadow, { 0, 0, 0, 0 });
	ImGui::SameLine(ImGui::GetWindowSize().x * 0.5f - 81);
	
	// TODO: Hay que arreglar esto, y Oriol aqui es donde se hacen los botones.
	if (ImGui::ImageButton((ImTextureID)buttonPlay->GetId(), { 27,18 }))
	{
		if (app->scene->GetGameState() == GameState::NOT_PLAYING) app->scene->Play();
		else app->scene->Stop();
	}
	ImGui::SameLine();
	if (ImGui::ImageButton((ImTextureID)buttonStop->GetId(), { 27,18 }))
	{
		if (app->scene->GetGameState() == GameState::PLAYING) app->scene->Pause();
		else if (app->scene->GetGameState() != GameState::NOT_PLAYING) app->scene->Resume();
	}
	ImGui::SameLine();
	if (ImGui::ImageButton((ImTextureID)buttonNextFrame->GetId(), { 27,18 }))
	{
		if (app->scene->GetGameState() == GameState::PAUSE) app->scene->NextFrame();
	}

	ImGui::PopStyleColor(3);
	ImGui::End();

	if (showMenu)
	{
		ImGui::ShowDemoWindow(&showMenu);
		ImGui::ShowMetricsWindow(&showMenu);
	}

	for (unsigned int i = 0; i < menus.size(); ++i)
	{
		if (menus[i]->active) menus[i]->Update(dt);
	}

	return true;
}

bool MainMenuBar::CleanUp()
{
	for (unsigned int i = 0; i < menus.size(); ++i)
	{
		RELEASE(menus[i]);
	}

	menus.clear();

	return true;
}

ConsoleMenu* MainMenuBar::GetConsole()
{
	if (!menus.empty())
	{
		ConsoleMenu* test = (ConsoleMenu*)menus[(int)Menus::CONSOLE];

		return test;
	}

	return nullptr;
}

std::string& MainMenuBar::GetCurrentDir()
{
	ContentBrowserMenu* content = (ContentBrowserMenu*)menus[(int)Menus::CONTENT_BROWSER];

	return content->GetCurrentDir();
}