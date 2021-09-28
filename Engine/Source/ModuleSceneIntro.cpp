#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "Primitive.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_opengl2.h"
#include "Imgui/imgui_impl_sdl.h"
#include <json\parson.h>

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
bool ModuleSceneIntro::Update(float dt)
{
	return true;
}

bool ModuleSceneIntro::PostUpdate()
{
	Plane p(0, 0, 0, 0);
	p.axis = true;
	p.Render();

	Cube c(1, 1, 1);
	c.wire = true;
	c.Render();

	return true;
}