#pragma once

#include "Component.h"

#include <string>
#include <vector>

class TransformComponent;

class Actions
{
public:
	Actions();
	~Actions();

private:
	std::string name;
	//std::vector<Bindings> bindings;
};

class ActionMaps
{
public:
	ActionMaps();
	~ActionMaps();

private:
	std::string name;
	std::vector<Actions> actions;
};

class InputActionComponent : Component
{
public:

	InputActionComponent(GameObject* own, TransformComponent* trans);
	~InputActionComponent();

	void OnEditor() override;
	bool Update(float dt) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

private:



};