#pragma once

#include "Component.h"

#include "Lights.h"

class ComponentLight : public Component
{
public:
	ComponentLight();
	virtual ~ComponentLight();

	bool Update(float dt) override;

	void OnEditor() override;

	void SetLight(Light* light);

	//JSON_Value* OnSave() override;
	//void OnLoad(JSON_Object* obj, GameObject* parent = nullptr) override;
	virtual bool OnLoad(JsonParsing& node) override;
	virtual bool OnSave(JsonParsing& node, JSON_Array* array) override;


private:
	Light* light;

};