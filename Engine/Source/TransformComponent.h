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
	void SetTranslation(float3 pos);
	void SetRotation(Quat rot);
	void SetScale(float3 sca);

	inline float4x4 GetTransform() const { return transform; }
	inline float3 GetPosition() const { return position; }

	Quat AngleToQuat(float angle, int x, int y, int z);


	void ShowTransformationInfo();
private:
	float3 position;
	Quat rotation;
	float3 scale;
	float4x4 transform;
};