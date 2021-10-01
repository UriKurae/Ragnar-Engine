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

	app->camera->Move(Vec3(1.0f, 1.0f, 0.0f));
	app->camera->LookAt(Vec3(0, 0, 0));

	Plane* p = new Plane(0, 0, 0, 0);
	p->axis = true;

	Cube* c = new Cube(1, 1, 1);
	c->SetPos(4.5, 0, 0);
	c->color = Color(1.0f, 0.0f, 0.0f, 0.2f);

	Cube* d = new Cube(1, 1, 1);
	d->SetPos(5, 0, 0);
	d->color = Color(0.0f, 0.0f, 1.0f, 0.1f);

	app->renderer3D->AddPrimitive(p);
	app->renderer3D->AddPrimitive(c);
	app->renderer3D->AddPrimitive(d);

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