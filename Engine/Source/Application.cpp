#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"

Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	sceneIntro = new ModuleSceneIntro();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	editor = new ModuleEditor();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	
	// Scenes
	AddModule(sceneIntro);
	AddModule(editor);

	AddModule(renderer3D);

	saveRequested = false;
}

Application::~Application()
{
	std::list<Module*>::iterator item;

	for (item = --listModules.end(); item != listModules.begin(); --item)
	{
		RELEASE(*item);
	}
}

bool Application::Init()
{
	bool ret = true;

	JSON_Value* root = jsonFile.ParseFile(CONFIG_FILENAME);

	if (jsonFile.GetRootValue() == NULL)
	{
		LOG("Couldn't load %s", CONFIG_FILENAME);
		ret = false;
	}

	// Call Init() in all modules
	std::list<Module*>::iterator item;

	for (item = listModules.begin(); item != listModules.end() && ret; ++item)
	{
		ret = (*item)->Init(jsonFile.GetChild(root, (*item)->name));
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
	if (saveRequested) SaveConfig();
}

// Call PreUpdate, Update and PostUpdate on all modules
bool Application::Update()
{
	bool ret = true;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = listModules.begin();
	
	for (item = listModules.begin(); item != listModules.end() && ret; ++item)
	{
		ret = (*item)->PreUpdate(dt);
	}


	for (item = listModules.begin(); item != listModules.end() && ret; ++item)
	{
		ret = (*item)->Update(dt);
	}


	for (item = listModules.begin(); item != listModules.end() && ret; ++item)
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

void Application::RequestBrowser(const char* path)
{
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
}

void Application::LogConsole(const char* string)
{
	editor->LogConsole(string);
}

void Application::SaveConfig()
{
	JSON_Value* root = jsonFile.GetRootValue();

	// Call Init() in all modules
	std::list<Module*>::iterator item;

	for (item = listModules.begin(); item != listModules.end(); ++item)
	{
		(*item)->SaveConfig(jsonFile.SetChild(root, (*item)->name));
	}

	jsonFile.SerializeFile(root, CONFIG_FILENAME);
	saveRequested = false;
}
