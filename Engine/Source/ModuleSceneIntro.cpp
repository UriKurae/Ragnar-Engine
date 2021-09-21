#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_opengl2.h"
#include "Imgui/imgui_impl_sdl.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
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
	ImGui::Begin("Ragnar Engine");
	if (ImGui::Button("Close", ImVec2(0,0)))
	{
		return UpdateStatus::UPDATE_STOP;
	}
	
	//if (ImGui::CollapsingHeader("Tab"))
	//{
	//	ImGui::TreeNode("Close");
	//}

	ImGui::End();

	return UpdateStatus::UPDATE_CONTINUE;
}