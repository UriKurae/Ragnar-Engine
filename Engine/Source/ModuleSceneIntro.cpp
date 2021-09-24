#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_opengl2.h"
#include "Imgui/imgui_impl_sdl.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool startEnabled) : Module(app, startEnabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	
	bool ret = true;

	app->camera->Move(Vec3(1.0f, 1.0f, 0.0f));
	app->camera->LookAt(Vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
UpdateStatus ModuleSceneIntro::Update(float dt)
{
	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleSceneIntro::PostUpdate()
{
	if (ImGui::BeginMainMenuBar())
	{
		bool ret = false;

		if (ImGui::BeginMenu("File"))
		{
			// Project options (Create, open...)
			ImGui::MenuItem("New Project", NULL, &ret);
			ImGui::MenuItem("Open Project", NULL, &ret);

			ImGui::Separator();

			ImGui::MenuItem("Save", "Ctrl + S", &ret);
			if (ImGui::MenuItem("Exit", "Alt + F4", &ret))
			{
				return UpdateStatus::UPDATE_STOP;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("Undo", "Ctrl + Z", &ret);
			ImGui::MenuItem("Redo", "Ctrl + Y", &ret);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("Demo Menu", NULL, &showMenu);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About Ragnar Engine", NULL, &showHelpMenu);
			ImGui::EndMenu();
		}
		
		ImGui::EndMainMenuBar();
	}
	if (showMenu)
	{
		ImGui::ShowDemoWindow(&showMenu);

		ImGui::Begin("Ragnar Engine", &showMenu);
		if (ImGui::Button("Close", ImVec2(0, 0)))
		{
			return UpdateStatus::UPDATE_STOP;
		}
		ImGui::End();

		//ImGui::Button("File", ImVec2(0, 0));
		//ImGui::Button("Edit", ImVec2(0, 0));

		//if (ImGui::CollapsingHeader("Tab"))
		//{
		//	ImGui::TreeNode("Close");
		//}
	}

	if (showHelpMenu)
	{	
		if (ImGui::Begin("About Ragnar Engine", &showHelpMenu))
		{
			ImGui::TextWrapped
			(
				"This is a videogame engine created for our game engine project."
				" We are two students, Lucas Perez and Oriol Bernal, and the final goal for this engine is"
				" to create a videogame."
			);
		}
		ImGui::End();
	}

	return UpdateStatus::UPDATE_CONTINUE;
}