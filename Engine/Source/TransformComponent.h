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

typedef float GLfloat;

class TransformComponent : public Component
{
public:
	TransformComponent(GameObject* owner);
	TransformComponent(TransformComponent* trans);
	~TransformComponent();

	bool Update(float dt) override;

	void OnEditor() override;

	void SetTransform(float3 pos, Quat rot, float3 sca);
	void SetTransform(float4x4 trMatrix);

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void UpdateTransform();
	void UpdateChildTransform(GameObject* go);
	void SetAABB();

	inline float4x4 GetLocalTransform() const { return localMatrix; }
	inline float4x4 GetGlobalTransform() const { return globalMatrix; }
	inline float3 GetPosition() const { return position; }
	inline Quat GetRotation() const { return rotation; }
	inline float3 GetScale() const { return scale; }
	inline float3 GetRotEditor() const { return rotationEditor; }

	bool DrawVec3(std::string& name, float3& vec);
	void ShowTransformationInfo();

private:
	float3 position;
	Quat rotation;
	float3 rotationEditor;
	float3 scale;
	float4x4 globalMatrix;
	float4x4 localMatrix;


	bool changeTransform;
};