#include "ModuleScene.h"

#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "Optick/include/optick.h"

ModuleScene::ModuleScene()
{
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Start()
{
	OPTICK_EVENT("Starting Scene");

	app->camera->position = Vec3(0.0f, 1.0f, -5.0f);
	app->camera->LookAt(Vec3(0, 0, 0));

	CreateGameObject();

	return true;
}

bool ModuleScene::Update(float dt)
{
	OPTICK_EVENT("Updating Scene");

	return true;
}

bool ModuleScene::PostUpdate()
{
	OPTICK_EVENT("Scene PostUpdate");

	return true;
}

GameObject* ModuleScene::CreateGameObject()
{
	OPTICK_EVENT("Creating Game Object");

	GameObject* object = new GameObject();
	gameObjects.emplace_back(object);
	
	return object;
}