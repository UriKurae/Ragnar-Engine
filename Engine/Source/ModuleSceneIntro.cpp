#include "Application.h"
#include "ModuleSceneIntro.h"

#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "Primitive.h"

#include <json\parson.h>

#include "mmgr/mmgr.h"

ModuleSceneIntro::ModuleSceneIntro(bool startEnabled) : Module(startEnabled)
{
	name = "SceneIntro";
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");

	bool ret = true;

	//app->camera->Move(Vec3(0.0f, 0.0f, 0.0f));
	app->camera->position = Vec3(0.0f, 1.0f, -5.0f);
	app->camera->LookAt(Vec3(0, 0, 0));

	//Plane* p = new Plane(0, 0, 0, 0);
	//p->axis = true;

	//app->renderer3D->AddPrimitive(p);


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
	if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_UP) app->SaveConfigRequest();

	return true;
}

bool ModuleSceneIntro::PostUpdate()
{

	return true;
}