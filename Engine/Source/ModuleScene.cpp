#include "ModuleScene.h"

#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "Optick/include/optick.h"

#include "mmgr/mmgr.h"

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

	for (int i = 0; i < gameObjects.size(); ++i)
	{
		gameObjects[i]->Draw();
	}

	return true;
}

bool ModuleScene::CleanUp()
{
	for (int i = 0; i < gameObjects.size(); ++i)
	{
		RELEASE(gameObjects[i]);
	}

	return true;
}

GameObject* ModuleScene::CreateGameObject()
{
	OPTICK_EVENT("Creating Game Object");

	GameObject* object = new GameObject();
	object->CreateComponent(ComponentType::TRANSFORM);
	gameObjects.emplace_back(object);
	
	return object;
}

void ModuleScene::MoveGameObjectUp(GameObject* object)
{
	if (object == gameObjects[0]) return;

	int size = gameObjects.size();
	for (int i = 0; i < size; ++i)
	{
		if (gameObjects[i] == object)
		{
			GameObject* aux = gameObjects[i];

			gameObjects[i] = gameObjects[i - 1];
			gameObjects[i - 1] = aux;
			break;
		}
	}
}

void ModuleScene::MoveGameObjectDown(GameObject* object)
{
	int size = gameObjects.size() - 1;
	if (object == gameObjects[size]) return;

	for (int i = size; i >= 0; --i)
	{
		if (gameObjects[i] == object)
		{
			GameObject* aux = gameObjects[i];

			gameObjects[i] = gameObjects[i + 1];
			gameObjects[i + 1] = aux;
			break;
		}
	}
}