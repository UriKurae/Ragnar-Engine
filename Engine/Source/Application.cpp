#include "Application.h"
#include <list>

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	sceneIntro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);


	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	
	// Scenes
	AddModule(sceneIntro);

	AddModule(renderer3D);
}

Application::~Application()
{
	std::list<Module*>::iterator item;

	for (item = --listModules.end(); item != listModules.begin(); --item)
	{
		delete *item;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::list<Module*>::iterator item;

	for (item = listModules.begin(); item != listModules.end() && ret; ++item)
	{
		ret = (*item)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	for (item = listModules.begin(); item != listModules.end() && ret; ++item)
	{
		ret = (*item)->Start();
	}
	
	return ret;
}


// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)msTimer.Read() / 1000.0f;
	msTimer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
UpdateStatus Application::Update()
{
	UpdateStatus ret = UpdateStatus::UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = listModules.begin();
	
	for (item = listModules.begin(); item != listModules.end() && ret == UpdateStatus::UPDATE_CONTINUE; ++item)
	{
		ret = (*item)->PreUpdate(dt);
	}


	for (item = listModules.begin(); item != listModules.end() && ret == UpdateStatus::UPDATE_CONTINUE; ++item)
	{
		ret = (*item)->Update(dt);
	}


	for (item = listModules.begin(); item != listModules.end() && ret == UpdateStatus::UPDATE_CONTINUE; ++item)
	{
		ret = (*item)->PostUpdate();
	}


	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::iterator item;

	for (item = --listModules.end(); item != listModules.begin() && ret; --item)
	{
		ret = (*item)->CleanUp();
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	listModules.push_back(mod);
}