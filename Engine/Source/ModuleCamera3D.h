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

	void Look(const Vec3 &pos, const Vec3 &ref, bool rotateAroundReference = false);
	void LookAt(const Vec3 &spot);
	void Move(const Vec3 &movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:

	Vec3 x, y, z, position, reference, objectSelected;

	// Ruben : aqui creo las matrices y la camara para tenerlas, la de view es 3x4 pero he probado tambien con 4x4
	Frustum* cameraFrustum;
	math::float4x4 projectionMatrix;
	math::float3x4 frustumViewMatrix;

private:

	Mat4x4 viewMatrix, viewMatrixInverse;
};