#pragma once
#include "Module.h"
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

private:
	bool showMenu = true;
	bool showHelpMenu = false;
};
