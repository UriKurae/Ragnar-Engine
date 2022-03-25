#include "MainMenuBar.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneManager.h"
#include "Scene.h"
#include "ModuleEditor.h"
#include "ModuleCamera3D.h"
#include "Physics3D.h"
#include "ModuleUI.h"

#include "ConsoleMenu.h"
#include "ConfigurationMenu.h"
#include "AboutMenu.h"
#include "InspectorMenu.h"
#include "HierarchyMenu.h"
#include "ContentBrowserMenu.h"
#include "TextEditorMenu.h"
#include "FogWarMenu.h"
#include "NavigatorMenu.h"

#include "TransformComponent.h"
#include "LightComponent.h"
#include "ButtonComponent.h"
#include "ImageComponent.h"
#include "CheckBoxComponent.h"
#include "SliderComponent.h"
#include "TextComponent.h"
#include "Transform2DComponent.h"

#include "CommandsDispatcher.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include "FileSystem.h"

#include "Lights.h"
#include "Texture.h"
#include "Dialogs.h"
#include "Viewport.h"
#include "Style.h"

#include "Math/float3x3.h"
#include "imgui/imgui_stdlib.h"

#include <fstream>
#include "Profiling.h"

MainMenuBar::MainMenuBar() : Menu(true, "MainMenu"), saveWindow(false), showBuildMenu(false), sceneSelected(nullptr)
{
	showMenu = false;

	menus.reserve(9);
	menus.emplace_back(new ConsoleMenu());
	menus.emplace_back(new ConfigurationMenu());
	menus.emplace_back(new AboutMenu());
	menus.emplace_back(new HierarchyMenu());
	menus.emplace_back(new ContentBrowserMenu());
	menus.emplace_back(new TextEditorMenu());
	menus.emplace_back(new FogWarMenu());
	menus.emplace_back(new NavigatorMenu());
	menus.emplace_back(new InspectorMenu()); // Inspector must be the LAST!!!

	stylesList = { "Deep Dark", "Red & Dark", "Green & Blue", "Classic Dark", "Visual Studio", "Dark Visual", "Gold & Black", "Smooth Dark" };
	iconList = { ICON_FA_WINDOW_MAXIMIZE, ICON_FA_WRENCH, ICON_FA_SITEMAP, ICON_FA_SITEMAP, ICON_FA_SITEMAP, ICON_FA_CODE, ICON_FA_CLOUD, ICON_FA_WALKING, ICON_FA_INFO_CIRCLE };
}

MainMenuBar::~MainMenuBar()
{
}

bool MainMenuBar::Start()
{
	for (int i = 0; i < menus.size(); ++i)
	{
		menus[i]->Start();
	}

	Style::SetStyle(style);

	return true;
}

bool MainMenuBar::Update(float dt)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (!FileMenu()) return false;		
		EditMenu();
		WindowMenu();
		ViewMenu();	
		GameObjectMenu();
		HelpMenu();
		SetStyleMenu();		
		
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

		std::string dir = app->sceneManager->GetCurrentScene()->GetAssetsPath();
		if (!dir.empty())
		{
			dir = dir.substr(dir.find("Output\\") + 7, dir.length());
		}
		ImGui::TextWrapped("%s", dir.empty() ? "Untitled" : dir.c_str());
		ImGui::NewLine();
		if (ImGui::Button("Save"))
		{
			if (app->sceneManager->GetCurrentScene()->GetAssetsPath().empty())
			{
				std::string filePath = Dialogs::SaveFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
				if (!filePath.empty()) app->sceneManager->GetCurrentScene()->SaveScene(filePath.c_str());
			}
			else
			{
				app->sceneManager->GetCurrentScene()->SaveScene(app->sceneManager->GetCurrentScene()->GetAssetsPath().c_str());
			}
			app->sceneManager->GetCurrentScene()->NewScene();
			saveWindow = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Don't Save"))
		{
			app->sceneManager->GetCurrentScene()->NewScene();
			saveWindow = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) saveWindow = false;
		ImGui::End();
	}

	PlayBar();

	// ShortCuts
	if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_LSHIFT) != KeyState::KEY_REPEAT && 
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
			app->sceneManager->ChangeScene(filePath.c_str());
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT)
	{
		if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KeyState::KEY_REPEAT)
		{
			if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_DOWN)
			{
				std::string filePath = Dialogs::SaveFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
				if (!filePath.empty()) app->sceneManager->GetCurrentScene()->SaveScene(filePath.c_str());
			}
			if (app->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_DOWN)
			{
				AlignWithView();
			}
			if (app->input->GetKey(SDL_SCANCODE_G) == KeyState::KEY_DOWN)
			{
				if (app->editor->GetGO() != nullptr) app->sceneManager->GetCurrentScene()->CreateGameObjectParent("GameObjectParent", app->editor->GetGO());
			}
			if (app->input->GetKey(SDL_SCANCODE_N) == KeyState::KEY_DOWN)
			{
				if (app->editor->GetGO() != nullptr) app->sceneManager->GetCurrentScene()->CreateGameObject(app->editor->GetGO());
				else app->sceneManager->GetCurrentScene()->CreateGameObject(nullptr);
			}
		}
		else if (app->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT)
		{
			if (app->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_DOWN)
			{
				AlignViewWithSelected();
			}
			if (app->input->GetKey(SDL_SCANCODE_N) == KeyState::KEY_DOWN)
			{
				if (app->editor->GetGO() != nullptr) app->sceneManager->GetCurrentScene()->CreateGameObjectChild("GameObjectChild", app->editor->GetGO());
			}
		}
	}
	
	else if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_DOWN)
	{
		if (app->sceneManager->GetCurrentScene()->GetAssetsPath().empty())
		{
			std::string filePath = Dialogs::SaveFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
			if (!filePath.empty()) app->sceneManager->GetCurrentScene()->SaveScene(filePath.c_str());
		}
		else app->sceneManager->GetCurrentScene()->SaveScene(app->sceneManager->GetCurrentScene()->GetAssetsPath().c_str());
	}

	// Build menu
	if (showBuildMenu)
	{
		static bool addScene = false;
		ImGui::Begin("Build", &showBuildMenu);

		ImGui::BeginChild("Scenes");
		for (int i = 0; i < app->sceneManager->GetScenes().size(); ++i)
		{
			int flags = ImGuiTreeNodeFlags_Leaf;
			flags |= sceneSelected == app->sceneManager->GetScenes()[i] ? ImGuiTreeNodeFlags_Selected : 0;
			ImGui::TreeNodeEx(app->sceneManager->GetScenes()[i]->GetName().c_str(), flags);
			if (ImGui::IsItemClicked())
			{
				sceneSelected = app->sceneManager->GetScenes()[i];
			}
			ImGui::TreePop();
		}

		if (ImGui::Button("+"))
		{
			addScene = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("-"))
		{
			app->sceneManager->DeleteScene(sceneSelected);
		}

		if (addScene)
		{
			ImGui::Begin("Add Scene");

			std::vector<std::shared_ptr<Scene>> scenes = ResourceManager::GetInstance()->GetScenes();
			static std::shared_ptr<Scene> scene = nullptr;
			
			for (int i = 0; i < scenes.size(); ++i)
			{
				int flags = ImGuiTreeNodeFlags_Leaf;
				flags |= scene == scenes[i] ? ImGuiTreeNodeFlags_Selected : 0;
				ImGui::TreeNodeEx(scenes[i]->GetName().c_str(), flags);
				if (ImGui::IsItemClicked())
				{
					scene = scenes[i];
				}
				ImGui::TreePop();
			}

			if (ImGui::Button("Add") && scene != nullptr)
			{
				app->sceneManager->AddScene(scene);
				app->sceneManager->SaveBuild();
				scene = nullptr;
				addScene = false;
			}

			ImGui::End();
		}

		ImGui::EndChild();

		ImGui::End();
	}

	ImGui::End();

	if (showMenu)
	{
		ImGui::ShowDemoWindow(&showMenu);
		ImGui::ShowMetricsWindow(&showMenu);
	}


	if (showCreateLightSensibleShaderWindow)
	{
		ShowCreateLigthSensibleShaderWindow();
	}
	else if (showCreateNotLightSensibleShaderWindow)
	{
		ShowCreateNotLigthSensibleShaderWindow();
	}

	for (unsigned int i = 0; i < menus.size(); ++i)
	{
		if (menus[i]->active) menus[i]->Update(dt);
	}

	return true;
}

bool MainMenuBar::FileMenu()
{
	if (ImGui::BeginMenu(ICON_FA_FILE" File"))
	{
		// Project options (Create, open...)
		if (ImGui::MenuItem(ICON_FA_FILE_UPLOAD" New Project", "Ctrl + N"))
			saveWindow = true;

		if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN" Open Project", "Ctrl + O"))
		{
			std::string filePath = Dialogs::OpenFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
			if (!filePath.empty()) app->sceneManager->GetCurrentScene()->LoadScene(filePath.c_str());
		}

		ImGui::Separator();

		if (ImGui::MenuItem(ICON_FA_SAVE" Save", "Ctrl + S"))
		{
			if (app->sceneManager->GetCurrentScene()->GetAssetsPath().empty())
			{
				std::string filePath = Dialogs::SaveFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
				if (!filePath.empty()) app->sceneManager->GetCurrentScene()->SaveScene(filePath.c_str());
			}
			else app->sceneManager->GetCurrentScene()->SaveScene(app->sceneManager->GetCurrentScene()->GetAssetsPath().c_str());
		}
		if (ImGui::MenuItem(ICON_FA_SAVE" Save As", "Ctrl + Shift + S"))
		{
			std::string filePath = Dialogs::SaveFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
			if (!filePath.empty()) app->sceneManager->GetCurrentScene()->SaveScene(filePath.c_str());
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Build"))
			showBuildMenu = true;

		if (ImGui::MenuItem(ICON_FA_WINDOW_CLOSE" Exit", "ESC"))
			return false;

		ImGui::EndMenu();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("Opens the file menu");
	}

	return true;
}

void MainMenuBar::EditMenu()
{
	if (ImGui::BeginMenu(ICON_FA_EDIT" Edit"))
	{
		if(ImGui::MenuItem(ICON_FA_UNDO" Undo", "Ctrl + Z"))
			CommandDispatcher::Undo();
		if(ImGui::MenuItem(ICON_FA_REDO" Redo", "Ctrl + Y"))
			CommandDispatcher::Redo();

		ImGui::Separator();
		app->editor->GetViewport()->SnapOptions();
		ImGui::EndMenu();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("Opens the edit menu");
	}
}

void MainMenuBar::WindowMenu()
{
	if (ImGui::BeginMenu(ICON_FA_WINDOW_RESTORE" Window"))
	{
		std::string iconName;
		for (size_t i = 0; i < menus.size(); i++)
		{
			iconName = iconList.at(i) + " " + menus.at(i)->name;
			ImGui::MenuItem(iconName.c_str(), NULL, &menus[i]->active);
		}

		ImGui::EndMenu();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("Opens the window menu");
	}
}

void MainMenuBar::ViewMenu()
{
	if (ImGui::BeginMenu(ICON_FA_EYE" View"))
	{
		if (ImGui::MenuItem("Depth Test", NULL, app->renderer3D->GetDepthTest()))
			app->renderer3D->SetDepthTest();
		if (ImGui::MenuItem("Cull Face", NULL, app->renderer3D->GetCullFace()))
			app->renderer3D->SetCullFace();
		if (ImGui::MenuItem("Lighting", NULL, app->renderer3D->GetLighting()))
			app->renderer3D->SetLighting();
		if (ImGui::MenuItem("Color Material", NULL, app->renderer3D->GetColorMaterial()))
			app->renderer3D->SetColorMaterial();
		if (ImGui::MenuItem("Texture 2D", NULL, app->renderer3D->GetTexture2D()))
			app->renderer3D->SetTexture2D();
		if (ImGui::MenuItem("Stencil", NULL, app->renderer3D->GetStencil()))
			app->renderer3D->SetStencil();
		if (ImGui::MenuItem("Blending", NULL, app->renderer3D->GetBlending()))
			app->renderer3D->SetBlending();
		if (ImGui::MenuItem("Wire", NULL, app->renderer3D->GetWireMode()))
			app->renderer3D->SetWireMode();
		if (ImGui::MenuItem("Show Raycast", NULL, app->renderer3D->GetRayCast()))
			app->renderer3D->SetWireMode();

		ImGui::MenuItem("Show NavMesh", NULL, app->renderer3D->GetNavMesh());
		ImGui::MenuItem("Show Grid", NULL, app->renderer3D->GetDrawGrid());
		ImGui::MenuItem("Show Quad Tree", NULL, app->sceneManager->GetCurrentScene()->GetDrawQuad());
		ImGui::EndMenu();
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("Opens the view menu");
	}
}

void MainMenuBar::GameObjectMenu()
{
	if (ImGui::BeginMenu(ICON_FA_PLUS" GameObject"))
	{
		CreateGameObjectMenu();
		ImGui::Separator();

		// Align with options
		if (app->editor->GetGO() == nullptr)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(128, 128, 128, 255));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(128, 128, 128, 100));
		}
		if (ImGui::MenuItem("Align with view", "Ctrl+Shift+F"))
			if (app->editor->GetGO()) AlignWithView();

		if (ImGui::MenuItem("Align view to selected", "Alt+Shift+F"))
			if (app->editor->GetGO()) AlignViewWithSelected();

		if (app->editor->GetGO() == nullptr)
		{
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		}

		ImGui::EndMenu();
	}
}

void MainMenuBar::CreateGameObjectMenu()
{
	// Empty Objects
	if (ImGui::MenuItem(ICON_FA_LAYER_GROUP" Create Empty Object", "Ctrl+Shift+N"))
	{
		if (app->editor->GetGO() != nullptr) app->sceneManager->GetCurrentScene()->CreateGameObject(app->editor->GetGO());
		else app->sceneManager->GetCurrentScene()->CreateGameObject(nullptr);
	}

	if (ImGui::MenuItem(ICON_FA_OBJECT_UNGROUP" Create Child", "Alt+Shift+N"))
		if (app->editor->GetGO() != nullptr) app->sceneManager->GetCurrentScene()->CreateGameObjectChild("GameObjectChild", app->editor->GetGO());

	if (ImGui::MenuItem(ICON_FA_OBJECT_GROUP" Create Parent", "Ctrl+Shift+G"))
		if (app->editor->GetGO() != nullptr) app->sceneManager->GetCurrentScene()->CreateGameObjectParent("GameObjectParent", app->editor->GetGO());

	// 3D Object
	if (ImGui::BeginMenu(ICON_FA_CUBES" Create 3D Object"))
	{
		static std::vector<std::string> primitives = { "Cube", "Pyramide", "Sphere", "Cylinder" };
		GameObject* selected = app->editor->GetGO();
		for (int i = 0; i < primitives.size(); i++)
		{
			if (ImGui::MenuItem(primitives.at(i).c_str()))
				app->sceneManager->GetCurrentScene()->Create3DObject((Object3D)i, selected);
		}

		ImGui::EndMenu();
	}
	// UI Object
	if (ImGui::BeginMenu(ICON_FA_CUBES" Create UI element"))
	{
		static std::vector<std::string> uiComponents = { "UI Button", "UI Image", "UI Check Box", "UI Slider", "UI Text" };
		for (int i = 0; i < uiComponents.size(); i++)
		{
			if (ImGui::MenuItem(uiComponents.at(i).c_str()))
			{
				GameObject* object = app->sceneManager->GetCurrentScene()->CreateGameObject(nullptr, false);
				object->CreateComponent((ComponentType)(i + (int)ComponentType::UI_BUTTON));
				object->SetName(uiComponents.at(i).c_str());
			}
		}

		ImGui::EndMenu();

	}
	// Lights
	if (ImGui::BeginMenu(ICON_FA_LIGHTBULB " Lights"))
	{
		if (ImGui::MenuItem("Point Light"))
		{
			GameObject* go = app->sceneManager->GetCurrentScene()->CreateGameObject(nullptr);
			ComponentLight* l = (ComponentLight*)go->CreateComponent(ComponentType::LIGHT);
			l->SetAsPointLight();
		}
		else if (ImGui::MenuItem("Spot Light"))
		{
			GameObject* go = app->sceneManager->GetCurrentScene()->CreateGameObject(nullptr);
			ComponentLight* l = (ComponentLight*)go->CreateComponent(ComponentType::LIGHT);
			l->SetAsSpotLight();
		}
		ImGui::EndMenu();
	}
	// Shader
	if (ImGui::BeginMenu(ICON_FA_CIRCLE " Shader"))
	{
		static int count = 0;
		if (ImGui::MenuItem("Light-sensible"))
			showCreateLightSensibleShaderWindow = true;

		else if (ImGui::MenuItem("Not light-sensible"))
			showCreateNotLightSensibleShaderWindow = true;

		ImGui::EndMenu();
	}
}

void MainMenuBar::HelpMenu()
{
	if (ImGui::BeginMenu(ICON_FA_INFO_CIRCLE" Help"))
	{
		ImGui::MenuItem("Demo Menu", NULL, &showMenu);
		ImGui::MenuItem(ICON_FA_USER" About Ragnar Engine", "", &menus[(int)Menus::ABOUT]->active);

		if (ImGui::MenuItem(ICON_FA_ADDRESS_BOOK" Documentation", "F1"))
			app->RequestBrowser("https://github.com/UriKurae/Ragnar-Engine");

		if (ImGui::MenuItem(ICON_FA_BUG" Report a Bug"))
			app->RequestBrowser("https://github.com/UriKurae/Ragnar-Engine/issues");
	
		if (ImGui::MenuItem(ICON_FA_DOWNLOAD" Download latest"))
			app->RequestBrowser("https://github.com/UriKurae/Ragnar-Engine/releases");
		
		ImGui::EndMenu();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("Opens the help menu");
	}
}

void MainMenuBar::SetStyleMenu()
{
	if (ImGui::BeginMenu(ICON_FA_BORDER_STYLE" Set Style"))
	{
		for (int i = 0; i < stylesList.size(); i++)
		{
			if (ImGui::MenuItem(stylesList.at(i).c_str()))
			{
				Style::SetStyle(i);
				style = i;
			}
		}
		ImGui::Separator();

		float auxAlpha = alphaStyle;
		ImGui::Text(ICON_FA_SORT_ALPHA_DOWN_ALT" PopUp Alpha:");
		ImGui::PushItemWidth(100);
		if (ImGui::InputFloat("##Alpha", &alphaStyle, 0.1f))
		{
			if (alphaStyle < auxAlpha)
				Style::SetAlpha(0.9);
			else Style::SetAlpha(1.1);
			alphaStyle = auxAlpha;
		}
		ImGui::PopItemWidth();
		ImGui::EndMenu();
	}
}

void MainMenuBar::PlayBar()
{
	bool ret = true;
	ImGui::Begin(" ", &ret, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 255 });
	ImGui::PushStyleColor(ImGuiCol_Border, { 0, 0, 0, 255 });
	ImGui::SameLine(ImGui::GetWindowSize().x * 0.5f - 81);
	GameState state = app->sceneManager->GetGameState();

	if (state == GameState::NOT_PLAYING)
	{
		if (ImGui::Button(ICON_FA_PLAY))
		{
			app->sceneManager->Play();
			AudioManager::Get()->PlayAllAudioSources();
			app->physics->ActiveAllBodies();
		}

		ImGui::SameLine();
		ImGui::Button(ICON_FA_PAUSE);
		ImGui::SameLine();
		ImGui::Button(ICON_FA_STEP_FORWARD);
	}
	else
	{
		if (ImGui::Button(ICON_FA_STOP))
		{
			AudioManager::Get()->StopAllAudioSources();
			app->sceneManager->Stop();
			app->physics->SleepAllBodies();
		}
		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_PAUSE))
		{
			if (state == GameState::PAUSE)
			{
				app->sceneManager->Resume();
				AudioManager::Get()->ResumeAllAudioSources();
				app->physics->ActiveAllBodies();
			}
			else
			{
				AudioManager::Get()->PauseAllAudioSources();
				app->sceneManager->Pause();
				app->physics->SleepAllBodies();
			}
		}
		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_STEP_FORWARD) && state == GameState::PAUSE)
			app->sceneManager->NextFrame(); // Not implemented yet
	}
	ImGui::PopStyleColor(2);
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

void MainMenuBar::SetStyle(int _style)
{
	style = _style; 
	Style::SetStyle(style);
}

// Object align with camera
void MainMenuBar::AlignWithView()
{
	GameObject* temp = app->editor->GetGO();
	if (temp != nullptr)
	{
		TransformComponent* transform = temp->GetComponent<TransformComponent>();
		float4x4 matrix = transform->GetGlobalTransform();
		Frustum frus = app->camera->cameraFrustum;
		matrix.SetTranslatePart(frus.Pos());
		float3x3 rot{ frus.WorldRight(), frus.Up(), frus.Front() };
		matrix.SetRotatePart(rot.ToQuat());
		transform->SetTransform(matrix);
	}
}

// Camera align with object
void MainMenuBar::AlignViewWithSelected()
{
	GameObject* temp = app->editor->GetGO();
	if (temp != nullptr)
	{
		TransformComponent* transform = temp->GetComponent<TransformComponent>();
		float4x4 matrix = transform->GetGlobalTransform();
		float3x3 rot = matrix.RotatePart();
		app->camera->cameraFrustum.SetFrame(transform->GetGlobalTransform().Col3(3), rot.Col3(2), rot.Col3(1));
	}
}

std::string MainMenuBar::GetNotLightSensibleShaderSource()
{
	return "#type vertex\n"
		"#version 430 core\n\n"

		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec3 normal;\n"
		"layout(location = 2) in vec2 texCoords;\n\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"uniform mat3 normalMatrix;\n\n"

		"out vec3 vPosition;\n"
		"out vec3 vNormal;\n"
		"out vec2 vTexCoords;\n\n"

		"void main()\n"
		"{\n\t"
			"gl_Position = projection * view * model * vec4(position, 1);\n\n\t"

			"vTexCoords = texCoords;\n\t"
			"vPosition = vec3(model * vec4(position, 1));\n\t"
			"vNormal = normalMatrix * normal;\n\t"
			"vNormal = normalize((model * vec4(normal, 0.0)).xyz);\n"
		"}\n\n\n"


		"#type fragment\n\n"
		"#version 430 core\n\n"

		"in vec3 vPosition;\n"
		"in vec3 vNormal;\n"
		"in vec2 vTexCoords;\n\n"

		"out vec4 fragColor;\n\n"

		"void main()\n"
		"{\n\t"
			"fragColor = vec4(1.0f);\n"
		"}\n";
}

std::string MainMenuBar::GetLightSensibleShaderSource()
{
	return R"(#type vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform float textureAlpha;
uniform vec3 ambientColor;
uniform vec3 camPos;

out vec3 vPosition;
out vec3 vAmbientColor;
out vec2 vTexCoords;
out vec3 vCamPos;
out vec3 vNormal;
out float vTextureAlpha;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1);

	vTexCoords = texCoords;
	vPosition = vec3(model * vec4(position, 1));
	//vNormal = normalMatrix * normal;
	vNormal = normalize((model * vec4(normal, 0.0)).xyz);
	vAmbientColor = ambientColor;
	vTextureAlpha = 1.0f;

	vCamPos = camPos;
}


#type fragment
#version 430 core

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;
in vec3 camPos;

in vec3 vAmbientColor;
in float vTextureAlpha;

out vec4 fragColor;

//uniform sampler2D tex;
layout(location = 0) uniform sampler2D tex;


struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	bool gammaCorrection;
	float gammaCorrectionAmount;
};

uniform Material material;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// 0 -> constant, 1 -> linear (lin), 2 -> quadratic
	vec3 properties;

	float intensity;
	float constant;
	float lin;
	float quadratic;
};
#define MAX_POINT_LIGHTS 4
uniform PointLight pointLights[MAX_POINT_LIGHTS];

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	float intensity;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};
#define MAX_SPOT_LIGHTS 4
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	vec3 ambient = light.ambient * material.diffuse * vAmbientColor;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// Attenuation
	float distance = length(fragPos - light.position);
	float attenuation = 1.0 / (1 + light.constant + light.lin * distance + light.quadratic * (distance * distance));
	//float attenuation = 1.0 / (light.constant + light.lin * distance + light.quadratic * (distance * distance));
	//float attenuation = 1.0 / (light.properties[0] + light.properties[1] * distance + light.properties[2] * (distance * distance));
	
	attenuation *= light.intensity;

	vec3 ambient = light.ambient * material.diffuse * vAmbientColor;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - fragPos);
	//float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// spotlight intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// combine results
	//vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	//vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	//vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	vec3 ambient = light.ambient * material.diffuse * material.diffuse;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;


	ambient *=  /*attenuation **/ intensity * light.intensity;
	diffuse  *= /*attenuation **/ intensity * light.intensity;
	specular *= /*attenuation **/ intensity * light.intensity;
	
	return (ambient + diffuse + specular);
}

void main()
{
	vec3 norm = normalize(vNormal);
	vec3 viewDir = normalize(camPos - vPosition);
	
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
		result += CalcPointLight(pointLights[i], norm, vPosition, viewDir);
	
	for (int i = 0; i < MAX_SPOT_LIGHTS; ++i)
		result += CalcSpotLight(spotLights[i], norm, vPosition, viewDir);

	vec3 finalColor = result;
	if (material.gammaCorrection)
	{
		finalColor = pow(result, vec3(1.0 / material.gammaCorrectionAmount));
	}

	fragColor = texture(tex , vTexCoords) * vTextureAlpha * vec4(finalColor, 1);
})";

}

void MainMenuBar::ShowCreateLigthSensibleShaderWindow()
{
	ImGui::Begin("Create Shader", &showCreateLightSensibleShaderWindow);

	ImGui::Dummy({ 10,2 });

	ImGui::Text("Assets/Resources/Shaders/");
	ImGui::SameLine();

	static std::string name;
	ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Assets/Resources/Shaders/").x - 15);
	ImGui::InputText("Shader Name", &name);
	ImGui::PopItemWidth();

	if (ImGui::Button("Create", { 50,25 }))
	{
		if (!name.empty())
		{
			std::string path = "Assets/Resources/Shaders/" + name + ".shader";

			std::ofstream file;
			file.open(path);
			file << GetLightSensibleShaderSource();
			file.close();

			ResourceManager::GetInstance()->CreateResource(ResourceType::SHADER, path, std::string());
			showCreateLightSensibleShaderWindow = false;
			name.clear();
		}
	}
	
	ImGui::End();
}

void MainMenuBar::ShowCreateNotLigthSensibleShaderWindow()
{
	ImGui::Begin("Create Shader", &showCreateNotLightSensibleShaderWindow);

	ImGui::Dummy({ 10,2 });

	ImGui::Text("Assets/Resources/Shaders/");
	ImGui::SameLine();

	static std::string name;
	ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Assets/Resources/Shaders/").x - 15);
	ImGui::InputText("Shader Name", &name);
	ImGui::PopItemWidth();

	if (ImGui::Button("Create", { 50,25 }))
	{
		if (!name.empty())
		{
			std::string path = "Assets/Resources/Shaders/" + name + ".shader";

			std::ofstream file;
			file.open(path);
			file << GetNotLightSensibleShaderSource();
			file.close();

			ResourceManager::GetInstance()->CreateResource(ResourceType::SHADER, path, std::string());

			showCreateNotLightSensibleShaderWindow = false;
			name.clear();
		}

	}

	ImGui::End();
	GameObject* temp = app->editor->GetGO();
	if (temp != nullptr)
	{
		TransformComponent* transform = temp->GetComponent<TransformComponent>();
		float4x4 matrix = transform->GetGlobalTransform();
		Frustum frus = app->camera->cameraFrustum;
		matrix.SetTranslatePart(frus.Pos());
		float3x3 rot{ frus.WorldRight(), frus.Up(), frus.Front() };
		matrix.SetRotatePart(rot.ToQuat());
		transform->SetTransform(matrix);
	}
}