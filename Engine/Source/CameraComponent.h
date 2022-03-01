#pragma once

#include "Component.h"
#include "MathGeoLib/src/Geometry/Frustum.h"

class TransformComponent;
class IndexBuffer;
class VertexBuffer;

class CameraComponent : public Component
{
public:
	CameraComponent(GameObject* own, TransformComponent* trans);
	~CameraComponent();

	void OnEditor() override;
	bool Update(float dt) override;
	void Draw() override;

	void SetPlanes();
	void CalculateVerticalFov(float horizontalFovRadians, float width, float height);
	void UpdateFovAndScreen(float width, float height);
	void UpdateFov();
	
	void CompileBuffers();
	bool CompareRotations(Quat& quat1, Quat& quat2);

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	int CameraComponent::ContainsAaBox(const AABB& boundingBox);
	Frustum* GetFrustum() { return &camera; };

	float4x4 matrixViewFrustum;
	float4x4 matrixProjectionFrustum;
private:

	Frustum camera;
	TransformComponent* transform;
	Quat currentRotation;
	float3 currentPos;

	float nearPlane;
	float farPlane;
	float verticalFov;
	float horizontalFov;
	float currentScreenWidth;
	float currentScreenHeight;

	IndexBuffer* ebo;
	VertexBuffer* vbo;

	//----------------------------------------------
	bool freeMovement = false;
	bool followTarget = false;
	GameObject*/*transform*/ target = nullptr;
	int s_lerp = 0;
	float verticalAngle = 0.79;
	bool rotateAround = false;
	float horizontalAngle = 0;
	bool multifocusOnClick = false;
	GameObject*/*transform*/secondTarget = nullptr;
public:
	void shake(float strength, float duration);
	void setTransition(std::vector<float> durations, std::vector<float3> positions);
private:
	float shakeStrength = 0;
	float shakeDuration = 0;
	std::vector<float> transitionDurations;
	std::vector<float3> transitionPositions;
	//----------------------------------------------

};