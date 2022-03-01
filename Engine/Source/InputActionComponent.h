#pragma once

#include "Component.h"

class TransformComponent;

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