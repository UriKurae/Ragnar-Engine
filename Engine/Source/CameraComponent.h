#pragma once
#include "Component.h"
#include "Geometry/Frustum.h"

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
	void Draw(CameraComponent* gameCam = nullptr) override;

	void SetPlanes();
	void CalculateVerticalFov(float horizontalFovRadians, float width, float height);
	void UpdateFovAndScreen(float width, float height);
	void UpdateFov();

	void SetNewFov(float horizontalFovRadians);
	
	void CompileBuffers();
	bool CompareRotations(Quat& quat1, Quat& quat2);

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	int CameraComponent::ContainsAaBox(const AABB& boundingBox);
	Frustum* GetFrustum() { return &camera; };

	inline float GetCurrentScreenWidth() { return currentScreenWidth; }
	inline float GetCurrentScreenHeight() { return currentScreenHeight; }

	float4x4 matrixViewFrustum;
	float4x4 matrixProjectionFrustum;

private:

	Frustum camera;

	TransformComponent* transform;
	Quat currentRotation;
	//float3 lastPos = float3(0, 0, 0);
	float3 originalPos;

private:

	float nearPlane;
	float farPlane;
	float verticalFov;
	float horizontalFov;

	float currentScreenWidth;
	float currentScreenHeight;

	IndexBuffer* ebo;
	VertexBuffer* vbo;

	//----------------------------------------------
	const float zoomMin = 10;
	const float zoomMax = 100;
	float zoom = zoomMax;

	bool fixingToTarget = false;

	bool freeMovement = true;
	GameObject* defTarget = nullptr; // FreeMovement Target
	float movementSpeed = 0.05f;
	bool followTarget = false;
	GameObject* target = nullptr;
	float verticalAngle = -40.0f;
	bool lockVerticalAngle = false;
	float rotationSpeed = 0.5f;
	float radius = 20.0f;
	float horizontalAngle = 0;
	
	// Controls
	bool rightClickRot = true; // Right click rotation and midle click movement go together
	bool arrowRot = false;
	bool midClickMov = true;
	bool WASDMov = false;
	bool borderMov = false;

	int targetUID = 0;

public:
	void RequestShake(float strength, float duration);
private:
	void Shake(float dt);

	float shakeStrength = 0.6f;
	float shakeDuration = 1.0f;
	bool shake = false;
	//bool smoothShake = true;
	int smooth = 0;
	float elapsedTime = 0.0f;
	float currentStrength = 0.0f;
};