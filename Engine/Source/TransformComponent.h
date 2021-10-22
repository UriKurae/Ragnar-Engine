#pragma once

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

typedef float GLfloat;

class TransformComponent : public Component
{
public:
	TransformComponent();
	~TransformComponent();

	bool Update(float dt) override;

	void OnEditor() override;

	void SetTransform(float3 pos, Quat rot, float3 sca);

	float4x4 GetTransform() const { return transform; }

	Quat AngleToQuat(float angle);

	void ShowTransformationInfo();
private:
	float3 position;
	Quat rotation;
	float3 scale;
	float rotations[3];
	float4x4 transform;
};