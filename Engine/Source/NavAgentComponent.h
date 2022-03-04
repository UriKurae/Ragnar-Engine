#pragma once

#include "Component.h"


class NavAgentComponent : public Component
{
public:
	NavAgentComponent();
	virtual ~NavAgentComponent();

	bool Update(float dt) override;

	void OnEditor() override;

	virtual bool OnLoad(JsonParsing& node) override;
	virtual bool OnSave(JsonParsing& node, JSON_Array* array) override;
};