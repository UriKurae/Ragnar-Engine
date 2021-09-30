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

	void SetFPSLimit(const float fps);
	inline int GetFPSLimit() const { return ((1.0f / (float)cappedMs) * 1000.0f); }

	inline void SaveConfigRequest() { saveRequested = true; }
	inline void LoadConfigRequest() { loadRequested = true; }

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void SaveConfig();
	void LoadConfig();
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* sceneIntro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;

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
};

extern Application* app;