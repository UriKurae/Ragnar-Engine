#pragma once
#include "Module.h"
#include "Geometry/Frustum.h"
#include "Geometry/Line.h"
#include <map>

class GameObject;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool startEnabled = true);
	~ModuleCamera3D();

	bool Start();
	bool Update(float dt) override;
	bool CleanUp();
	
	void MousePicking(math::float3& newPos, math::float3& newFront, float speed);
	void ThrowRayCast(std::vector<GameObject*>& gameObjects, math::LineSegment& picking, std::map<float, GameObject*>& triangleMap, float3& hitPoint);
	void ThrowRayCastOnlyOBB(std::vector<GameObject*>& gameObjects, math::LineSegment& picking, std::map<float, GameObject*>& aabbMap, float3& hitPoint);
	void OrbitAround(float& dt, math::float3& newUp, math::float3& newFront, math::float3& newPos);
	void RotateAround(float dt, math::float3& newFront, math::float3& newUp);
	void Focus(math::float3& newFront, math::float3& newUp, math::float3& newPos);

	void CalculateVerticalFov(float horizontalFovRadians, float width, float height);
	void CalculateViewMatrix();
	
	void UpdateFovAndScreen(float width, float height);
	void UpdateFov();
	void SetPlanes();

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) override;

	void LookAt(float3& target);
	int ContainsAaBox(const AABB& boundingBox);

public:
	Frustum cameraFrustum;

	float nearPlane;
	float farPlane;
	float verticalFov;
	float horizontalFov;
	float currentScreenWidth;
	float currentScreenHeight;

	float4x4 matrixViewFrustum;
	float4x4 matrixProjectionFrustum;

	math::Line rayCastToDraw;
	
	bool updateViewPort;
	bool updateGameView;
	bool visualizeFrustum;
};