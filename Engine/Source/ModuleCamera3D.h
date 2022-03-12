#pragma once
#include "Module.h"
#include "Geometry/Frustum.h"
#include "Geometry/Line.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool startEnabled = true);
	~ModuleCamera3D();

	bool Start();
	bool Update(float dt) override;
	void Focus(math::float3& newFront, math::float3& newUp, math::float3& newPos);
	bool CleanUp();

	void CalculateVerticalFov(float horizontalFovRadians, float width, float height);
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
	
	bool canBeUpdated;
	bool visualizeFrustum;
};