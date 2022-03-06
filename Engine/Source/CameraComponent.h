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
	//float3 lastPos = float3(0, 0, 0);
	float3 originalPos;

	float nearPlane;
	float farPlane;
	float verticalFov;
	float horizontalFov;
	float currentScreenWidth;
	float currentScreenHeight;

	IndexBuffer* ebo;
	VertexBuffer* vbo;

	//----------------------------------------------
	bool freeMovement = true;
	GameObject*/*transform*/ defTarget = nullptr; // FreeMovement Target
	float movementSpeed = 0.5f;
	bool followTarget = false;
	GameObject*/*transform*/ target = nullptr;
	int s_lerp = 0;
	float verticalAngle = -40.0f;
	bool lockVerticalAngle = false;
	float rotationSpeed = 0.5f;
	float radius = 20.0f;
	float horizontalAngle = 0;
	bool multifocusOnClick = false;
	GameObject*/*transform*/secondTarget = nullptr;

public:
	void RequestShake(float strength, float duration);
	void setTransition(std::vector<float> durations, std::vector<float3> positions);
private:
	void Shake(float dt);

	float shakeStrength = 0.6f;
	float shakeDuration = 1.0f;
	bool shake = false;
	//bool smoothShake = true;
	int smooth = 0;
	float elapsedTime = 0.0f;
	float currentStrength = 0.0f;
	std::vector<float> transitionDurations;
	std::vector<float3> transitionPositions;
};