#include "Application.h"
#include "Globals.h"
#include "AudioManager.h"
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
#include "FogWarMenu.h"
#include "Texture.h"
#include "TextureImporter.h"
#include "ResourceManager.h"
#include "ModuleEditor.h"
#include "NavigatorMenu.h"

#include "FileSystem.h"

#include "LightComponent.h"

#include "ModuleCamera3D.h"
#include "Physics3D.h"


#include "Dialogs.h"
#include "IconsFontAwesome5.h"
#include "Style.h"

#include "imgui/imgui_stdlib.h"

#include <fstream>

#include "Profiling.h"

#include "Math/float3x3.h"

MainMenuBar::MainMenuBar() : Menu(true), saveWindow(false), buttonPlay(nullptr), buttonPause(nullptr), buttonNextFrame(nullptr), buttonStop(nullptr), buttonPauseBlue(nullptr)
{
	showMenu = false;

	menus.reserve(8);
	menus.emplace_back(new ConsoleMenu());
	menus.emplace_back(new ConfigurationMenu());
	menus.emplace_back(new AboutMenu());
	menus.emplace_back(new HierarchyMenu());
	menus.emplace_back(new ContentBrowserMenu());
	menus.emplace_back(new FogWarMenu());
	menus.emplace_back(new NavigatorMenu());
	menus.emplace_back(new InspectorMenu()); // Inspector must be the LAST!!!

	stylesList = { "Deep Dark", "Red & Dark", "Green & Blue", "Classic Dark", "Visual Studio", "Dark Visual", "Gold & Black", "Smooth Dark" };
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

	Style::SetStyle(style);

	return true;
}

bool MainMenuBar::Update(float dt)
{
	if (ImGui::BeginMainMenuBar())
	{
		bool ret = false;

		if (ImGui::BeginMenu(ICON_FA_FILE" File"))
		{
			// Project options (Create, open...)
			if (ImGui::MenuItem(ICON_FA_FILE_UPLOAD" New Project", "Ctrl + N", &ret))
			{
				saveWindow = true;
			}
			if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN" Open Project", "Ctrl + O", &ret))
			{
				std::string filePath = Dialogs::OpenFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
				if (!filePath.empty()) app->scene->LoadScene(filePath.c_str());
			}

			ImGui::Separator();

			if (ImGui::MenuItem(ICON_FA_SAVE" Save", "Ctrl + S", &ret))
			{
				if (app->scene->SceneDirectory().empty())
				{
					std::string filePath = Dialogs::SaveFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
					if (!filePath.empty()) app->scene->SaveScene(filePath.c_str());
				}
				else app->scene->SaveScene(app->scene->SceneDirectory().c_str());
			}
			if (ImGui::MenuItem(ICON_FA_SAVE" Save As", "Ctrl + Shift + S", &ret))
			{
				std::string filePath = Dialogs::SaveFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
				if (!filePath.empty()) app->scene->SaveScene(filePath.c_str());
			}
			if (ImGui::MenuItem(ICON_FA_WINDOW_CLOSE" Exit", "ESC", &ret))
			{
				return false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the file menu");
		}

		if (ImGui::BeginMenu(ICON_FA_EDIT" Edit"))
		{
			ImGui::MenuItem(ICON_FA_UNDO" Undo", "Ctrl + Z", &ret);
			ImGui::MenuItem(ICON_FA_REDO" Redo", "Ctrl + Y", &ret);
			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the edit menu");
		}

		if (ImGui::BeginMenu(ICON_FA_WINDOW_RESTORE" Window"))
		{
			ImGui::MenuItem(ICON_FA_WINDOW_MAXIMIZE" Console", NULL, &GetConsole()->active);
			ImGui::MenuItem(ICON_FA_WRENCH" Configuration", NULL, &menus[(int)Menus::CONFIGURATION]->active);
			ImGui::MenuItem(ICON_FA_SITEMAP" Hierarchy", NULL, &menus[(int)Menus::HIERARCHY]->active);
			ImGui::MenuItem(ICON_FA_INFO_CIRCLE" Inspector", NULL, &menus[(int)Menus::INSPECTOR]->active);
			ImGui::MenuItem(ICON_FA_CLOUD" Fog War", NULL, &menus[(int)Menus::FOGWAR]->active);
			ImGui::MenuItem(ICON_FA_WALKING" Navigator", NULL, &menus[(int)Menus::NAVIGATOR]->active);

			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the window menu");
		}

		if (ImGui::BeginMenu(ICON_FA_EYE" View"))
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
			if (ImGui::MenuItem("Show NavMesh", NULL, app->renderer3D->GetNavMesh()))
			{

			}
			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the view menu");
		}

		if (ImGui::BeginMenu(ICON_FA_PLUS" GameObject"))
		{
			if (ImGui::MenuItem(ICON_FA_LAYER_GROUP" Create Empty Object", "Ctrl+Shift+N"))
			{
				if (app->editor->GetGO() != nullptr) app->scene->CreateGameObject(app->editor->GetGO());
				else app->scene->CreateGameObject(nullptr);
			}
      
			if (ImGui::MenuItem(ICON_FA_OBJECT_UNGROUP" Create Child", "Alt+Shift+N"))
			{
				if (app->editor->GetGO() != nullptr) app->scene->CreateGameObjectChild("GameObjectChild", app->editor->GetGO());
			}
			if (ImGui::MenuItem(ICON_FA_OBJECT_GROUP" Create Parent", "Ctrl+Shift+G"))
			{
				if (app->editor->GetGO() != nullptr) app->scene->CreateGameObjectParent("GameObjectParent", app->editor->GetGO());
			}
      
			if (ImGui::BeginMenu(ICON_FA_CUBES" Create 3D Object"))
			{
				if (ImGui::MenuItem("Cube"))
				{
					if (app->editor->GetGO() != nullptr) app->scene->Create3DObject(Object3D::CUBE, app->editor->GetGO());
					else app->scene->Create3DObject(Object3D::CUBE, nullptr);
				}
				else if (ImGui::MenuItem("Pyramide"))
				{
					if (app->editor->GetGO() != nullptr) app->scene->Create3DObject(Object3D::PYRAMIDE, app->editor->GetGO());
					else app->scene->Create3DObject(Object3D::PYRAMIDE, nullptr);
				}
				else if (ImGui::MenuItem("Sphere"))
				{
					if (app->editor->GetGO() != nullptr) app->scene->Create3DObject(Object3D::SPHERE, app->editor->GetGO());
					else app->scene->Create3DObject(Object3D::SPHERE, nullptr);
				}
				else if (ImGui::MenuItem("Cylinder"))
				{
					if (app->editor->GetGO() != nullptr) app->scene->Create3DObject(Object3D::CYLINDER, app->editor->GetGO());
					else app->scene->Create3DObject(Object3D::CYLINDER, nullptr);
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu(ICON_FA_LIGHTBULB " Lights"))
			{
				if (ImGui::MenuItem("Point Light"))
				{
					GameObject* go = app->scene->CreateGameObject(nullptr);
					go->SetName("Point Light");
					ComponentLight* l = (ComponentLight*)go->CreateComponent(ComponentType::LIGHT);
					PointLight* pl = new PointLight();
					l->SetLight(pl);
					app->renderer3D->AddPointLight(pl);

				}
				else if (ImGui::MenuItem("Spot Light"))
				{
					GameObject* go = app->scene->CreateGameObject(nullptr);
					go->SetName("Spot Light");
					ComponentLight* l = (ComponentLight*)go->CreateComponent(ComponentType::LIGHT);
					SpotLight* sl = new SpotLight();
					l->SetLight(sl);
					app->renderer3D->AddSpotLight(sl);

				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu(ICON_FA_CIRCLE " Shader"))
			{
				static int count = 0;
				if (ImGui::MenuItem("Light-sensible"))
				{
					showCreateLightSensibleShaderWindow = true;
				}
				else if (ImGui::MenuItem("Not light-sensible"))
				{					
					showCreateNotLightSensibleShaderWindow = true;
				}

				ImGui::EndMenu();
			}


			ImGui::Separator();
			if (app->editor->GetGO() == nullptr)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(128, 128, 128, 255));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(128, 128, 128, 100));
			}
			if (ImGui::MenuItem("Align with view", "Ctrl+Shift+F"))
			{
				if (app->editor->GetGO()) AlignWithView();
			}
			if (ImGui::MenuItem("Align view to selected", "Alt+Shift+F"))
			{
				if (app->editor->GetGO()) AlignViewWithSelected();
			}
			if (app->editor->GetGO() == nullptr)
			{
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
			}

			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu(ICON_FA_INFO_CIRCLE" Help"))
		{
			ImGui::MenuItem("Demo Menu", NULL, &showMenu);
			ImGui::MenuItem(ICON_FA_USER" About Ragnar Engine", "", &menus[(int)Menus::ABOUT]->active);
			if (ImGui::MenuItem(ICON_FA_ADDRESS_BOOK" Documentation", "F1", &ret))
			{
				app->RequestBrowser("https://github.com/UriKurae/Ragnar-Engine");
			}
			if (ImGui::MenuItem(ICON_FA_BUG" Report a Bug", "", &ret))
			{
				app->RequestBrowser("https://github.com/UriKurae/Ragnar-Engine/issues");
			}
			if (ImGui::MenuItem(ICON_FA_DOWNLOAD" Download latest", "", &ret))
			{
				app->RequestBrowser("https://github.com/UriKurae/Ragnar-Engine/releases");
			}
			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the help menu");
		}
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
			AudioManager::Get()->PlayAllAudioSources();
			//ImGui::StyleColorsClassic();
			app->physics->ActiveAllBodies();
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
			AudioManager::Get()->StopAllAudioSources();
			app->scene->Stop();
			app->physics->SleepAllBodies();
			//SetStyle(6);
		}
		ImGui::SameLine();

		if (app->scene->GetGameState() == GameState::PAUSE)
		{
			if (ImGui::ImageButton((ImTextureID)buttonPauseBlue->GetId(), { 27,18 }))
			{
				app->scene->Resume();
				AudioManager::Get()->ResumeAllAudioSources();
				app->physics->ActiveAllBodies();
			}
		}
		else if (ImGui::ImageButton((ImTextureID)buttonPause->GetId(), { 27,18 }))
		{
			AudioManager::Get()->PauseAllAudioSources();
			app->scene->Pause();
			app->physics->SleepAllBodies();
		}

		ImGui::SameLine();
		if (ImGui::ImageButton((ImTextureID)buttonNextFrame->GetId(), { 27,18 })) if (app->scene->GetGameState() == GameState::PAUSE) app->scene->NextFrame();

	}

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
			app->scene->LoadScene(filePath.c_str());
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT)
	{
		if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KeyState::KEY_REPEAT)
		{
			if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_DOWN)
			{
				std::string filePath = Dialogs::SaveFile("Ragnar Scene (*.ragnar)\0*.ragnar\0");
				if (!filePath.empty()) app->scene->SaveScene(filePath.c_str());
			}
			if (app->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_DOWN)
			{
				AlignWithView();
			}
			if (app->input->GetKey(SDL_SCANCODE_G) == KeyState::KEY_DOWN)
			{
				if (app->editor->GetGO() != nullptr) app->scene->CreateGameObjectParent("GameObjectParent", app->editor->GetGO());
			}
			if (app->input->GetKey(SDL_SCANCODE_N) == KeyState::KEY_DOWN)
			{
				if (app->editor->GetGO() != nullptr) app->scene->CreateGameObject(app->editor->GetGO());
				else app->scene->CreateGameObject(nullptr);
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
				if (app->editor->GetGO() != nullptr) app->scene->CreateGameObjectChild("GameObjectChild", app->editor->GetGO());
			}
		}
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

	else if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT)
	{
		
	}

	ImGui::PopStyleColor(3);
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