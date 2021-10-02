#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"

#include "mmgr/mmgr.h"

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

	listModules.clear();
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

	JsonParsing application = jsonFile.GetChild(root, "App");

	cappedMs = application.GetJsonNumber("FPS");

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
	frameCount++;
	lastSecFrameCount++;

	dt = (float)msTimer.Read() / 1000.0f;
	msTimer.Start();

	if (resizeRequested)
	{
		renderer3D->OnResize(*window->GetWindowWidth(), *window->GetWindowHeight());
		resizeRequested = false;
	}
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (loadRequested) LoadConfig();
	if (saveRequested) SaveConfig();

	if (lastSecFrameTime.Read() > 1000)
	{
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
		lastSecFrameTime.Start();
	}

	unsigned int lastFrameMs = msTimer.Read();

	if ((cappedMs > 0) && (lastFrameMs < cappedMs))
	{
		SDL_Delay(cappedMs - lastFrameMs);
	}
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

void Application::SetFPSLimit(const float fps)
{
	if (fps > 0) cappedMs = 1000 / fps;
	else cappedMs = 0;
}

void Application::SaveConfig()
{
	LOG("Saving configuration");

	JSON_Value* root = jsonFile.GetRootValue();

	JsonParsing application = jsonFile.SetChild(root, "App");

	application.SetNewJsonNumber(application.ValueToObject(application.GetRootValue()), "FPS", cappedMs);

	// Call Init() in all modules
	std::list<Module*>::iterator item;

	for (item = listModules.begin(); item != listModules.end(); ++item)
	{
		(*item)->SaveConfig(jsonFile.SetChild(root, (*item)->name));
	}

	jsonFile.SerializeFile(root, CONFIG_FILENAME);
	saveRequested = false;
}

void Application::LoadConfig()
{
	LOG("Loading configuration");

	JSON_Value* root = jsonFile.GetRootValue();

	JsonParsing application = jsonFile.GetChild(root, "App");

	cappedMs = application.GetJsonNumber("FPS");

	// Call Init() in all modules
	std::list<Module*>::iterator item;

	for (item = listModules.begin(); item != listModules.end(); ++item)
	{
		(*item)->LoadConfig(jsonFile.GetChild(root, (*item)->name));
	}

	loadRequested = false;
}