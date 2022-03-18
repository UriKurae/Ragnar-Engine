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

	bool Update(float dt) override;
	void CalculateOffsetPos();
	void UpdateMovement(bool mouseDragRight, float horizontalDrag);

	void Draw(CameraComponent* gameCam = nullptr) override;

	void OnEditor() override;
	void OnEditorMovement();
	void OnEditorShake();

	void SetPlanes();
	void CalculateVerticalFov(float horizontalFovRadians, float width, float height);
	void UpdateFovAndScreen(float width, float height);
	void UpdateFov();

	void SetNewFov(float horizontalFovRadians);
	
	void CompileBuffers();
	bool CompareRotations(Quat& quat1, Quat& quat2);
	void RequestShake(float strength, float duration);

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	int CameraComponent::ContainsAaBox(const AABB& boundingBox);
	inline Frustum* GetFrustum() { return &camera; };

	inline float GetCurrentScreenWidth() { return currentScreenWidth; }
	inline float GetCurrentScreenHeight() { return currentScreenHeight; }

	float4x4 matrixViewFrustum;
	float4x4 matrixProjectionFrustum;

private:
	void Shake(float dt);

	Frustum camera;
	TransformComponent* transform;
	Quat currentRotation;
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
	bool followTarget = false;
	bool lockVerticalAngle = false;

	GameObject* defTarget = nullptr; // FreeMovement Target
	GameObject* target = nullptr;

	float movementSpeed = 0.05f;
	float verticalAngle = -40.0f;
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

	float shakeStrength = 0.6f;
	float shakeDuration = 1.0f;
	bool shake = false;
	int smooth = 0;
	float elapsedTime = 0.0f;
	float currentStrength = 0.0f;
};