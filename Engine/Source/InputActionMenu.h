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

	inline std::string GetName() { return name; }
	inline void AddBinding(int bind) { bindings.push_back(bind); }
	inline std::vector<int> GetBindings() { return bindings; }

private:
	std::string name;
	std::vector<int> bindings;
};

class ActionMaps
{
public:
	ActionMaps();
	~ActionMaps();

	void OnSave(JsonParsing& node, JSON_Array* array);
	void OnLoad(JsonParsing& node);

	inline std::string GetName() { return name; }
	inline std::vector<Actions*>* GetActions() { return &actions; }

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

	int currentMap;
	int currentAction;
	int currentBinding;
	const char* currentBindingItem = NULL;
};