#pragma once

#include "Component.h"
#include "ModuleNavMesh.h"

struct NavAgentProperties {
	float speed = 0.0f;
	float angularSpeed = 0.0f;
	float stoppingDistance = 0.0f;
};

class NavAgentComponent : public Component
{
public:
	NavAgentComponent(GameObject* own, TransformComponent* trans);
	virtual ~NavAgentComponent();

	bool Update(float dt) override;

	void OnEditor() override;

	virtual bool OnLoad(JsonParsing& node) override;
	virtual bool OnSave(JsonParsing& node, JSON_Array* array) override;

	std::vector<float3> path;
	NavAgentProperties properties;

private:
	NavAgent* selectedNav = nullptr;

};