#pragma once
#include "Component.h"

struct NavAgent;

class NavAgentComponent : public Component
{
public:
	NavAgentComponent(GameObject* obj);
	virtual ~NavAgentComponent();

	bool Update(float dt) override;
	void OnEditor() override;

	virtual bool OnLoad(JsonParsing& node) override;
	virtual bool OnSave(JsonParsing& node, JSON_Array* array) override;

private:
	NavAgent* agentProperties = nullptr;
};