#pragma once
#include "Module.h"

#include "glmath.h"
#include "Light.h"

#include <vector>
#include "Primitive.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool startEnabled = true);
	~ModuleRenderer3D();

	bool Init(JsonParsing& node) override;
	bool PreUpdate(float dt) override;
	bool PostUpdate();
	bool CleanUp();

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

	void OnResize(int width, int height);

	void SetDepthTest();
	void SetCullFace();
	void SetLighting();
	void SetColorMaterial();
	void SetTexture2D();
	void SetStencil();
	void SetBlending();
	void SetWireMode();

	inline bool* GetDepthTest() { return &depthTest; }
	inline bool* GetCullFace() { return &cullFace; }
	inline bool* GetLighting() { return &lighting; }
	inline bool* GetColorMaterial() { return &colorMaterial; }
	inline bool* GetTexture2D() { return &texture2D; }
	inline bool* GetStencil() { return &stencil; }
	inline bool* GetBlending() { return &blending; }
	inline bool* GetWireMode() { return &wireMode; }

	void AddPrimitive(Primitive* primitive);

public:
	std::vector<Primitive*> primitives;

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	Mat3x3 normalMatrix;
	Mat4x4 modelMatrix, viewMatrix, projectionMatrix;

	bool depthTest;
	bool cullFace;
	bool lighting;
	bool colorMaterial;
	bool texture2D;
	bool stencil;
	bool blending;
	
	bool wireMode;
};