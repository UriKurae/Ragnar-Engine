#pragma once

#include "Component.h"
#include <vector>

class TransformComponent;

class Bindings
{
public:
	Bindings();
	~Bindings();

private:



};

class Actions
{
public:
	Actions();
	~Actions();

private:

	std::vector<Bindings> bindings;

};

class InputActionComponent : Component
{
public:
	InputActionComponent(GameObject* own, TransformComponent* trans);
	~InputActionComponent();

private:

};