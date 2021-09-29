#pragma once
#include "Module.h"

#define BOUNCER_TIME 200

struct PhysBody3D;
class Cube;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool startEnabled = true);
	~ModuleSceneIntro();

	bool Start();
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp();

private:
	bool showMenu = true;
	bool showHelpMenu = false;
};
