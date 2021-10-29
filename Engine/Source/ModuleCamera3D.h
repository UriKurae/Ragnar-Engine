#pragma once
#include "Module.h"
#include "glmath.h"
#include "MathGeoLib/src/Geometry/Frustum.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool startEnabled = true);
	~ModuleCamera3D();

	bool Start();
	bool Update(float dt) override;
	bool CleanUp();

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

	void LookAt(float3& target);

public:
	
	Vec3 x, y, z, position, reference, objectSelected;

	Frustum cameraFrustum;

	float4x4 matrixViewFrustum;
	float4x4 matrixProjectionFrustum;

private:

	Mat4x4 viewMatrix, viewMatrixInverse;
};