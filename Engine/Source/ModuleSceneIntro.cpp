#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
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

	app->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	app->camera->LookAt(vec3(0, 0, 0));

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
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleSceneIntro::PostUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_P) == KeyState::KEY_UP) isPaused = !isPaused;

	if (!isPaused)
	{
		ImGui::ShowDemoWindow();

		ImGui::Begin("Ragnar Engine");
		if (ImGui::Button("Close", ImVec2(0, 0)))
		{
			return UpdateStatus::UPDATE_STOP;
		}
		ImGui::End();

		//ImGui::Button("File", ImVec2(0, 0));
		//ImGui::Button("Edit", ImVec2(0, 0));

		if (ImGui::BeginMainMenuBar())
		{
			bool ret = false;
			if (ImGui::BeginMenu("File"))
			{
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
			ImGui::EndMainMenuBar();
		}

		//if (ImGui::CollapsingHeader("Tab"))
		//{
		//	ImGui::TreeNode("Close");
		//}
	}

	return UpdateStatus::UPDATE_CONTINUE;
}