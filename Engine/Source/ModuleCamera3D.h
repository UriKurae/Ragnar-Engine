#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool startEnabled = true);
	~ModuleCamera3D();

	bool Start();
	UpdateStatus Update(float dt) override;
	bool CleanUp();

	void Look(const vec3 &pos, const vec3 &ref, bool rotateAroundReference = false);
	void LookAt(const vec3 &spot);
	void Move(const vec3 &movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	vec3 x, y, z, position, reference;

private:

	mat4x4 viewMatrix, viewMatrixInverse;
};