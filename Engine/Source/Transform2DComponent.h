#pragma once
#include "glmath.h"
#include "Component.h"
#include "Math/float4x4.h"
#include <vector>

class GameObject;

class ComponentTransform2D : public Component
{
public:
	ComponentTransform2D(float3 pos, float3 sca, float3 rot, GameObject* Own);
	~ComponentTransform2D();
	
	bool Update(float dt)override;
	void OnEditor() override
		;
	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;
	
	Quat FromEulerToQuat(float3 eulerAngles);
	float3 FromQuatToEuler(Quat quatAngles);

	inline float3 GetPosition() { return position; };
	inline float3 GetInternalPosition() { return internalPosition; };
	inline float3 GetScale() { return scale; };
	inline Quat GetRotationQuat() { return rotationQuat; };

	inline float GetButtonWidth() { return buttonWidth; };
	inline float GetButtonHeight() { return buttonHeight; };

private:
	std::vector<float3> vertices_aux;
	float4x4 transMatrix;
	float4x4 transmat;

	float3 rotationEuler;
	float3 position;
	float3 internalPosition;
	float3 scale;
	Quat rotationQuat;
	
	float buttonWidth;
	float buttonHeight;
};
