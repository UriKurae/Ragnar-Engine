#pragma once
#include "Module.h"

#include "glmath.h"
#include "Light.h"

#include <vector>
#include "Primitive.h"
#include "glew/include/GL/glew.h"

#include "Model.h"

#define MAX_LIGHTS 8

typedef unsigned int GLuint;

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
	void SetVsync();

	inline bool* GetDepthTest() { return &depthTest; }
	inline bool* GetCullFace() { return &cullFace; }
	inline bool* GetLighting() { return &lighting; }
	inline bool* GetColorMaterial() { return &colorMaterial; }
	inline bool* GetTexture2D() { return &texture2D; }
	inline bool* GetStencil() { return &stencil; }
	inline bool* GetBlending() { return &blending; }
	inline bool* GetWireMode() { return &wireMode; }
	inline bool* GetVsync() { return &vsync; }

	void DrawCubeDirectMode();
	void DrawMeshes();

	void AddPrimitive(Primitive* primitive);

	void InitMesh(const char* filePath);
	void DrawMesh();

public:
	std::vector<Primitive*> primitives;

	std::vector<GLushort> indices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texcoords;
	std::vector<GLfloat> vertices;

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	Mat3x3 normalMatrix;
	Mat4x4 modelMatrix, viewMatrix, projectionMatrix;

	//LoadFBX fbx;
	std::vector<Model> fbx;

	GLuint framebuffer;
	GLuint rboDepthStencil;

	bool depthTest;
	bool cullFace;
	bool lighting;
	bool colorMaterial;
	bool texture2D;
	bool stencil;
	bool blending;
	bool wireMode;
	bool vsync;
};