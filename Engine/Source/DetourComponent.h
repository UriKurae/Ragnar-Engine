#pragma once

#include "Component.h"


class DetourComponent : public Component
{
public:
	DetourComponent();
	virtual ~DetourComponent();

	bool Update(float dt) override;

	void OnEditor() override;

	virtual bool OnLoad(JsonParsing& node) override;
	virtual bool OnSave(JsonParsing& node, JSON_Array* array) override;
};