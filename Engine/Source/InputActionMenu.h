#pragma once

#include "Menu.h"

#include <string>
#include <vector>

class Actions
{
public:
	Actions();
	~Actions();

	void OnSave(JsonParsing& node, JSON_Array* array);
	void OnLoad(JsonParsing& node);

private:
	std::string name;
	//std::vector<Bindings> bindings;
};

class ActionMaps
{
public:
	ActionMaps();
	~ActionMaps();

	void OnSave(JsonParsing& node, JSON_Array* array);
	void OnLoad(JsonParsing& node);

private:
	std::string name;
	std::vector<Actions*> actions;
};

class InputActionMenu : public Menu
{
public:
	InputActionMenu(); 
	~InputActionMenu();

	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	bool SaveInputActionFile(const char* path);
	bool LoadInputActionFile(const char* path);

private:

	std::vector<ActionMaps*> actionMaps;

};