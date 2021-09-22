#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"

class Application
{
public:
	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
	bool CleanUp();

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* sceneIntro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;

private:
	Timer	msTimer;
	float	dt;
	p2List<Module*> listModules;
};