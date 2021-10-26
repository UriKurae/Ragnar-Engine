#pragma once

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

typedef float GLfloat;

class TransformComponent : public Component
{
public:
	TransformComponent(GameObject* owner);
	~TransformComponent();

	bool Update(float dt) override;

	void OnEditor() override;

	void SetTransform(float3 pos, Quat rot, float3 sca);
	void SetTranslation(float3 pos);
	void SetRotation(Quat rot);
	void SetScale(float3 sca);

	void SetChildTransform(float3 pos, Quat rot, float3 sca);
	void SetParentTransform(TransformComponent* component);
	void RecursiveTransform(GameObject* parent);

	inline float4x4 GetTransform() const { return globalMatrix; }
	inline float3 GetPosition() const { return position; }
	inline Quat GetRotation() const { return rotation; }
	inline float3 GetScale() const { return scale; }

	Quat AngleToQuat(float angle, int x, int y, int z);


	void ShowTransformationInfo();
private:
	float3 position;
	Quat rotation;
	float3 scale;
	float4x4 globalMatrix;

	bool changeTransform;
	float rotationEditor[3];
};