#pragma once

#include "Timer.h"
#include "Module.h"

#include <list>
#include <string>

#include "JsonParsing.h"

class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
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

	void SetFPSLimit(const float fps);
	inline int GetFPSLimit() const { return ((1.0f / (float)cappedMs) * 1000.0f); }

	inline void SaveConfigRequest() { saveRequested = true; }
	inline void LoadConfigRequest() { loadRequested = true; }

	inline void ResizeRequest() { resizeRequested = true; }

	inline const char* GetAppName() const { return APPLICATION_NAME; }
	inline const char* GetOrganizationName() const { return ORGANIZATION_NAME; }

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void ReadConfiguration(JsonParsing& node);
	void SaveConfiguration(JsonParsing& node);

	void SaveConfig();
	void LoadConfig();
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* sceneIntro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	FileSystem* fs;

	JsonParsing jsonFile;

private:
	std::list<Module*> listModules;

	Timer msTimer;
	Timer lastSecFrameTime;
	unsigned int lastSecFrameCount = 0;
	unsigned int prevLastSecFrameCount = 0;
	unsigned int frameCount = 0;

	float dt;
	int cappedMs = -1;

	bool saveRequested;
	bool loadRequested;
	bool resizeRequested;
};

extern Application* app;