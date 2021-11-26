#pragma once

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

enum class Operation
{
	TRANSLATE,
	ROTATE,
	SCALE
};

enum class Mode
{
	LOCAL,
	WORLD
};

enum class RotationOrder
{
	XYZ = 0,
	ZYX,
	YXZ
};

typedef float GLfloat;

class TransformComponent : public Component
{
public:
	TransformComponent(GameObject* owner);
	~TransformComponent();

	bool Update(float dt) override;

	void OnEditor() override;

	void SetTransform(float3 pos, Quat rot, float3 sca);
	void SetTransform(float4x4 trMatrix);
	void SetTranslation(float3 pos);
	void SetRotation(Quat rot);
	void SetScale(float3 sca);

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void SetChildTransform(float3 pos, Quat rot, float3 sca);
	void SetParentTransform(TransformComponent* component);
	void RecursiveTransform(GameObject* parent);

	void UpdateTransform();
	void UpdateChildTransform(GameObject* go);
	void SetAABB();

	inline float4x4 GetLocalTransform() const { return localMatrix; }
	inline float4x4 GetGlobalTransform() const { return globalMatrix; }
	inline float3 GetPosition() const { return position; }
	inline Quat GetRotation() const { return rotation; }
	inline float3 GetScale() const { return scale; }

	bool DrawVec3(std::string& name, float3& vec);
	void ShowTransformationInfo();

private:
	float3 position;
	Quat rotation;
	float3 scale;
	float4x4 globalMatrix;
	float4x4 localMatrix;

	float3 rotationEditor;
	RotationOrder rotationOrder;


	bool changeTransform;
};