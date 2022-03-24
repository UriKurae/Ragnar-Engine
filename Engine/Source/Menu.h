#pragma once
#include "Imgui/imgui.h"
#include "IconsFontAwesome5.h"
#include <string>

class Menu
{
public:
	Menu(bool act, std::string name) : active(act), name(name) {}
	virtual ~Menu(){}

	virtual bool Start() { return true; }
	virtual bool Update(float dt) { return true; }
	virtual bool CleanUp() { return true; }
	
	bool* GetActive() { return &active; }
public:
	bool active;
	std::string name;
};