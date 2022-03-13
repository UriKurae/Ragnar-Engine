#include "ModuleSceneManager.h"
#include "Scene.h"

ModuleSceneManager::ModuleSceneManager(bool startEnabled) : Module(startEnabled)
{
	currentScene = new Scene();
	AddScene(currentScene);
}

ModuleSceneManager::~ModuleSceneManager()
{
}

bool ModuleSceneManager::Start()
{
	for (int i = 0; i < scenes.size(); ++i)
	{
		scenes[i]->Start();
	}
	return true;
}

bool ModuleSceneManager::PreUpdate(float dt)
{
	for (int i = 0; i < scenes.size(); ++i)
	{
		scenes[i]->PreUpdate(dt);
	}
	return true;
}

bool ModuleSceneManager::Update(float dt)
{
	for (int i = 0; i < scenes.size(); ++i)
	{
		scenes[i]->Update(dt);
	}
	return true;
}

bool ModuleSceneManager::PostUpdate()
{
	for (int i = 0; i < scenes.size(); ++i)
	{
		scenes[i]->PostUpdate();
	}
	return true;
}

bool ModuleSceneManager::Draw()
{
	for (int i = 0; i < scenes.size(); ++i)
	{
		scenes[i]->Draw();
	}
	return true;
}

bool ModuleSceneManager::CleanUp()
{
	for (int i = 0; i < scenes.size(); ++i)
	{
		scenes[i]->CleanUp();
	}
	return true;
}

void ModuleSceneManager::NewScene()
{
	currentScene->NewScene();
}

void ModuleSceneManager::AddScene(Scene* newScene)
{
	scenes.push_back(newScene);
}
