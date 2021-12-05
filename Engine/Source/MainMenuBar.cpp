#include "Application.h"
#include "Globals.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
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

#include "Dialogs.h"

#include "Profiling.h"

MainMenuBar::MainMenuBar() : Menu(true), saveWindow(false), buttonPlay(nullptr), buttonPause(nullptr), buttonNextFrame(nullptr), buttonStop(nullptr), buttonPauseBlue(nullptr)
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
	//TextureImporter::ImportTexture2(std::string("Assets/Resources/PauseButtonActive.png"));
	//TextureImporter::ImportTexture2(std::string("Assets/Resources/StopButton.png"));
	
	buttonPlay = new Texture(-5, std::string("Settings/EngineResources/PlayButton.rgtexture"));
	buttonPlay->Load();

	buttonStop = new Texture(-6, std::string("Settings/EngineResources/StopButton.rgtexture"));
	buttonStop->Load();

	buttonPause = new Texture(-7, std::string("Settings/EngineResources/PauseButton.rgtexture"));
	buttonPause->Load();

	buttonPauseBlue = new Texture(-8, std::string("Settings/EngineResources/PauseButtonActive.rgtexture"));
	buttonPauseBlue->Load();

	buttonNextFrame = new Texture(-9, std::string("Settings/EngineResources/NextFrame.rgtexture"));
	buttonNextFrame->Load();

	for (int i = 0; i < menus.size(); ++i)
	{
		menus[i]->Start();
	}

	return true;
}

bool MainMenuBar::Update(float dt)
{
	if (ImGui::BeginMainMenuBar())
	{
		bool ret = false;

		if (ImGui::BeginMenu("File"))
		{
			// Project options (Create, open...)
			if (ImGui::MenuItem("New Project", "Ctrl + N", &ret))
			{
				saveWindow = true;
			}
			if (ImGui::MenuItem("Open Project", "Ctrl + O", &ret))
			{
				std::string filePath = Dialogs::OpenFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
				if (!filePath.empty()) app->scene->LoadScene(filePath.c_str());
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save", "Ctrl + S", &ret))
			{
				if (app->scene->SceneDirectory().empty())
				{
					std::string filePath = Dialogs::SaveFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
					if (!filePath.empty()) app->scene->SaveScene(filePath.c_str());
				}
				else app->scene->SaveScene(app->scene->SceneDirectory().c_str());
			}
			if (ImGui::MenuItem("Save As", "Ctrl + Shift + S", &ret))
			{
				std::string filePath = Dialogs::SaveFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
				if (!filePath.empty()) app->scene->SaveScene(filePath.c_str());
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
			if (ImGui::MenuItem("Show Raycast", NULL, app->renderer3D->GetRayCast()))
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

	if (saveWindow)
	{
		bool saved = true;
		ImVec2 size = { 200, 100 };
		ImVec2 position = { (float)(app->window->width / 2) - 100, (float)(app->window->height / 2) - 50 };
		ImGui::SetNextWindowPos(position);
		ImGui::Begin("Ask for Save", &saved, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::TextWrapped("Do you want to save the changes you made in: ");

		std::string dir = app->scene->SceneDirectory();
		if (!dir.empty())
		{
			dir = dir.substr(dir.find("Output\\") + 7, dir.length());
		}
		ImGui::TextWrapped("%s", dir.empty() ? "Untitled" : dir.c_str());
		ImGui::NewLine();
		if (ImGui::Button("Save"))
		{
			if (app->scene->SceneDirectory().empty())
			{
				std::string filePath = Dialogs::SaveFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
				if (!filePath.empty()) app->scene->SaveScene(filePath.c_str());
			}
			else
			{
				app->scene->SaveScene(app->scene->SceneDirectory().c_str());
			}
			app->scene->NewScene();
			saveWindow = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Don't Save"))
		{
			app->scene->NewScene();
			saveWindow = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) saveWindow = false;
		ImGui::End();
	}

	bool ret = true;
	ImGui::Begin(" ", &ret, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
	ImGui::PushStyleColor(ImGuiCol_Border, { 0, 0, 0, 0 });
	ImGui::PushStyleColor(ImGuiCol_BorderShadow, { 0, 0, 0, 0 });
	ImGui::SameLine(ImGui::GetWindowSize().x * 0.5f - 81);
	
	if (app->scene->GetGameState() == GameState::NOT_PLAYING)
	{
		if (ImGui::ImageButton((ImTextureID)buttonPlay->GetId(), { 27,18 }))
		{
			app->scene->Play();
			ImGui::StyleColorsClassic();
		}

		ImGui::SameLine();
		ImGui::ImageButton((ImTextureID)buttonPause->GetId(), { 27,18 });

		ImGui::SameLine();
		ImGui::ImageButton((ImTextureID)buttonNextFrame->GetId(), { 27,18 });

	}
	else if (app->scene->GetGameState() == GameState::PLAYING || app->scene->GetGameState() == GameState::PAUSE)
	{
		if (ImGui::ImageButton((ImTextureID)buttonStop->GetId(), { 27,18 }))
		{
			app->scene->Stop();
			StyleTheme();
		}
		ImGui::SameLine();

		if (app->scene->GetGameState() == GameState::PAUSE)
		{
			if (ImGui::ImageButton((ImTextureID)buttonPauseBlue->GetId(), { 27,18 })) app->scene->Resume();
		}
		else 
			if (ImGui::ImageButton((ImTextureID)buttonPause->GetId(), { 27,18 })) app->scene->Pause();


		ImGui::SameLine();
		if (ImGui::ImageButton((ImTextureID)buttonNextFrame->GetId(), { 27,18 })) if (app->scene->GetGameState() == GameState::PAUSE) app->scene->NextFrame();

	}

	if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_N) == KeyState::KEY_DOWN)
	{
		saveWindow = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_O) == KeyState::KEY_DOWN)
	{
		std::string filePath = Dialogs::OpenFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
		if (!filePath.empty())
		{
			app->scene->LoadScene(filePath.c_str());
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_DOWN)
	{
		std::string filePath = Dialogs::SaveFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
		if (!filePath.empty()) app->scene->SaveScene(filePath.c_str());
	}
	else if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_DOWN)
	{
		if (app->scene->SceneDirectory().empty())
		{
			std::string filePath = Dialogs::SaveFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
			if (!filePath.empty()) app->scene->SaveScene(filePath.c_str());
		}
		else app->scene->SaveScene(app->scene->SceneDirectory().c_str());
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

	RELEASE(buttonPlay);
	RELEASE(buttonStop);
	RELEASE(buttonPause);
	RELEASE(buttonPauseBlue);
	RELEASE(buttonNextFrame);

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

void MainMenuBar::StyleTheme()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImColor(7, 7, 10);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
	colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	colors[ImGuiCol_FrameBg] = ImColor(74, 82, 90);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImColor(7, 7, 10);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_Button] = ImColor(0, 75, 168);
	colors[ImGuiCol_ButtonHovered] = ImColor(62, 120, 178);
	colors[ImGuiCol_ButtonActive] = ImColor(62, 120, 178);
	colors[ImGuiCol_Header] = ImColor(0, 75, 168);
	colors[ImGuiCol_HeaderHovered] = ImColor(62, 120, 178);
	colors[ImGuiCol_HeaderActive] = ImColor(0, 75, 168);
	colors[ImGuiCol_Separator] = ImColor(62, 120, 178);
	colors[ImGuiCol_SeparatorHovered] = ImColor(0, 75, 168);
	colors[ImGuiCol_SeparatorActive] = ImColor(62, 120, 178);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_DragDropTarget] = ImColor(255, 0, 0);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(8.00f, 8.00f);
	style.FramePadding = ImVec2(5.00f, 2.00f);
	style.CellPadding = ImVec2(6.00f, 6.00f);
	style.ItemSpacing = ImVec2(6.00f, 6.00f);
	style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
	style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	style.IndentSpacing = 25;
	style.ScrollbarSize = 15;
	style.GrabMinSize = 10;
	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = 1;
	style.TabBorderSize = 1;
	style.WindowRounding = 7;
	style.ChildRounding = 4;
	style.FrameRounding = 3;
	style.PopupRounding = 4;
	style.ScrollbarRounding = 9;
	style.GrabRounding = 3;
	style.LogSliderDeadzone = 4;
	style.TabRounding = 4;
}