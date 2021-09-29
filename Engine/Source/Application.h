#pragma once

#include "Timer.h"
#include "Module.h"

#include <list>
#include <string>

#include "json/parson.h"

class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleEditor;

#define CONFIG_FILENAME	"config.json"

class Application
{
public:
	Application();
	~Application();

	bool Init();
	bool Update();
	bool CleanUp();

	void RequestBrowser(const char* path);
	void LogConsole(const char* string);

	JSON_Value* LoadConfig(std::string fileName) const;

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* sceneIntro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;

private:
	Timer	msTimer;
	float	dt;
	std::list<Module*> listModules;
};

extern Application* app;