#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"

#define BOUNCER_TIME 200

struct PhysBody3D;
class Cube;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool startEnabled = true);
	~ModuleSceneIntro();

	bool Start();
	UpdateStatus Update(float dt) override;
	UpdateStatus PostUpdate() override;
	bool CleanUp();

	bool isPaused = false;

public:
};
