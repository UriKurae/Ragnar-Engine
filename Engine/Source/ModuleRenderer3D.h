#pragma once
#include "Module.h"

#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool startEnabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool PreUpdate(float dt) override;
	bool PostUpdate();
	bool CleanUp();

	void OnResize(int width, int height);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	Mat3x3 normalMatrix;
	Mat4x4 modelMatrix, viewMatrix, projectionMatrix;
};