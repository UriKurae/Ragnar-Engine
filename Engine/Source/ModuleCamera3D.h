#pragma once

#include "Module.h"

#include "MathGeoLib/src/Geometry/Frustum.h"
#include "MathGeoLib/src/Geometry/Line.h"

class VertexBuffer;
class IndexBuffer;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool startEnabled = true);
	~ModuleCamera3D();

	bool Start();
	bool Update(float dt) override;
	bool CleanUp();

	void CalculateVerticalFov(float horizontalFovRadians, float width, float height);
	void UpdateFovAndScreen(float width, float height);
	void UpdateFov();
	void SetPlanes();

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

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