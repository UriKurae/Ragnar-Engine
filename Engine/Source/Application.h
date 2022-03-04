#pragma once
#include "EngineTimer.h"
#include <list>
#include "JsonParsing.h"

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleScene;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleEditor;
class FileSystem;
class Physics3D;

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
	ModuleWindow* window = nullptr;
	Physics3D* physics = nullptr;
	ModuleInput* input = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleScene* scene = nullptr;
	FileSystem* fs = nullptr;

	JsonParsing jsonFile;

private:
	std::list<Module*> listModules;

	EngineTimer engineTimer;

	bool saveRequested;
	bool loadRequested;
	bool resizeRequested;
};

extern Application* app;