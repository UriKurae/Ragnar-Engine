#pragma once
#include "Component.h"

class Light;

class ComponentLight : public Component
{
public:
	ComponentLight();
	virtual ~ComponentLight();

	bool Update(float dt) override;
	void OnEditor() override;

	void SetLight(Light* light);

	virtual bool OnLoad(JsonParsing& node) override;
	virtual bool OnSave(JsonParsing& node, JSON_Array* array) override;

private:
	Light* light = nullptr;
};