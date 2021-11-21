#include "Application.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "FileSystem.h"
#include "ResourceManager.h"

#include "Profiling.h"

Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	scene = new ModuleScene();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	editor = new ModuleEditor();

	fs = new FileSystem(RESOURCES_FOLDER);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	
	// Scenes
	AddModule(scene);
	AddModule(editor);

	AddModule(renderer3D);

	loadRequested = false;
	saveRequested = false;
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item;

	for (item = listModules.rbegin(); item != listModules.rend(); ++item)
	{
		RELEASE(*item);
	}

	RELEASE(fs);
	ResourceManager::ReleaseInstance();

	listModules.clear();
}

bool Application::Init()
{
	bool ret = true;

	char* buffer = nullptr;
	fs->Load(SETTINGS_FOLDER "config.json", &buffer);

	if (buffer != nullptr)
	{
		JsonParsing jsonFile((const char*)buffer);
		jsonFile.ValueToObject(jsonFile.GetRootValue());

		engineTimer.ReadConfig(jsonFile.GetChild(jsonFile.GetRootValue(), "App"));
		std::list<Module*>::iterator item;

		for (item = listModules.begin(); item != listModules.end() && ret; ++item)
		{
			ret = (*item)->Init(jsonFile.GetChild(jsonFile.GetRootValue(), (*item)->name));
		}

		RELEASE_ARRAY(buffer);
	}

	//// Call Init() in all modules
	std::list<Module*>::iterator item;

	// After all Init calls we call Start() in all modules
	DEBUG_LOG("Application Start --------------");

	for (item = listModules.begin(); item != listModules.end() && ret; ++item)
	{
		ret = (*item)->Start();
	}

	return ret;
}


// ---------------------------------------------
void Application::PrepareUpdate()
{
	engineTimer.Start();

	if (resizeRequested)
	{
		renderer3D->OnResize(*window->GetWindowWidth(), *window->GetWindowHeight());
		resizeRequested = false;
	}
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	RG_PROFILING_FUNCTION("Finishing Update");

	if (loadRequested) LoadConfig();
	if (saveRequested) SaveConfig();

	engineTimer.FinishUpdate();
}

// Call PreUpdate, Update and PostUpdate on all modules
bool Application::Update()
{
	bool ret = true;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = listModules.begin();
	
	for (item = listModules.begin(); item != listModules.end() && ret; ++item)
	{
		ret = (*item)->PreUpdate(engineTimer.GetDeltaTime());
	}

	for (item = listModules.begin(); item != listModules.end() && ret; ++item)
	{
		ret = (*item)->Update(engineTimer.GetDeltaTime());
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
	std::list<Module*>::reverse_iterator item;

	for (item = listModules.rbegin(); item != listModules.rend(); ++item)
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
	if (editor) editor->LogConsole(string);
}

void Application::SetFPSLimit(const int fps)
{
	/*if (fps > 0) cappedMs = 1000 / fps;
	else cappedMs = 0;*/
	if (fps > 0) engineTimer.SetDesiredCappedMs(1000 / fps);
	else engineTimer.SetDesiredCappedMs(0);
}

void Application::SaveConfig()
{
	DEBUG_LOG("Saving configuration");

	/*JSON_Value* root = jsonFile.GetRootValue();

	JsonParsing application = jsonFile.SetChild(root, "App");*/

	JsonParsing jsonFile;

	engineTimer.SaveConfig(jsonFile.SetChild(jsonFile.GetRootValue(), "App"));

	// Call Init() in all modules
	std::list<Module*>::iterator item;

	for (item = listModules.begin(); item != listModules.end(); ++item)
	{
		(*item)->SaveConfig(jsonFile.SetChild(jsonFile.GetRootValue(), (*item)->name));
	}

	char* buf;
	uint size = jsonFile.Save(&buf);

	if (fs->Save(SETTINGS_FOLDER CONFIG_FILENAME, buf, size) > 0)
		DEBUG_LOG("Saved Engine Preferences");

	RELEASE_ARRAY(buf);

	//jsonFile.SerializeFile(root, CONFIG_FILENAME);
	saveRequested = false;
}

void Application::LoadConfig()
{
	DEBUG_LOG("Loading configuration");

	char* buffer = nullptr;
	fs->Load(SETTINGS_FOLDER "config.json", &buffer);

	if (buffer != nullptr)
	{
		JsonParsing jsonFile((const char*)buffer);
		jsonFile.ValueToObject(jsonFile.GetRootValue());

		engineTimer.ReadConfig(jsonFile.GetChild(jsonFile.GetRootValue(), "App"));

		std::list<Module*>::iterator item;

		for (item = listModules.begin(); item != listModules.end(); ++item)
		{
			(*item)->LoadConfig(jsonFile.GetChild(jsonFile.GetRootValue(), (*item)->name));
		}

		RELEASE_ARRAY(buffer);
	}

	loadRequested = false;
}