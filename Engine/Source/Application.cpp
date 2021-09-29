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

	JSON_Value* root_value = LoadConfig(CONFIG_FILENAME);

	if (root_value == NULL)
	{
		LOG("No config file was found, creating one...");

		root_value = json_value_init_object();

		if (root_value != NULL)
		{
			LOG("File initialized");
		}
		else
		{
			LOG("Couldn't initialize the json file");
		}

		JSON_Object* root_object = json_value_get_object(root_value);
	
		json_object_set_value(root_object, "Window", json_value_init_object());
		JSON_Object* window = json_object_get_object(root_object, "Window");
		json_object_set_string(window, "Height", "1024");
	}

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

	json_serialize_to_file(root_value, CONFIG_FILENAME);

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

JSON_Value* Application::LoadConfig(std::string fileName) const
{
	JSON_Value* config = json_parse_file(fileName.c_str());

	return config;
}
