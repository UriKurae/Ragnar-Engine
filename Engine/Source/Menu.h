#pragma once

#include "Imgui/imgui.h"

class Menu
{
public:
	Menu(bool act) : active(act) {}
	virtual ~Menu(){}

	virtual bool Start() { return true; }
	virtual bool Update(float dt) { return true; }
	virtual bool CleanUp() { return true; }
	
	bool* GetActive() { return &active; }
public:
	bool active;
};