#pragma once

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

class TransformComponent : public Component
{
public:
	TransformComponent();
	~TransformComponent();

	bool Update(float dt) override;

	void OnEditor() override;

	void SetTransform(float3 pos, Quat rot, float3 sca);
private:
	float3 position;
	Quat rotation;
	float3 scale;
};