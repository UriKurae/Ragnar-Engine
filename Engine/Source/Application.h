#pragma once

#include "EngineTimer.h"
#include "Module.h"

#include <list>
#include <string>

#include "JsonParsing.h"

class ModuleWindow;
class ModuleInput;
class ModuleScene;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleEditor;
class FileSystem;

#define CONFIG_FILENAME	"config.json"
#define APPLICATION_NAME "Ragnar Engine"
#define ORGANIZATION_NAME "Yggdrasil Studios"

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

	void SetFPSLimit(const int fps);
	inline int GetFPSLimit() const { return engineTimer.GetFps(); }
	inline float GetEngineDeltaTime() const { return engineTimer.GetDeltaTime(); }

	inline void SaveConfigRequest() { saveRequested = true; }
	inline void LoadConfigRequest() { loadRequested = true; }

	inline void ResizeRequest() { resizeRequested = true; }

	inline const char* GetAppName() const { return APPLICATION_NAME; }
	inline const char* GetOrganizationName() const { return ORGANIZATION_NAME; }

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void SaveConfig();
	void LoadConfig();

public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleScene* scene;
	FileSystem* fs;

	JsonParsing jsonFile;

private:
	std::list<Module*> listModules;

	EngineTimer engineTimer;

	bool saveRequested;
	bool loadRequested;
	bool resizeRequested;
};

extern Application* app;