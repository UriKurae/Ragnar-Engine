#pragma once
#include "Math/float2.h"
#include "Math/float4.h"
#include "Imgui/ImGuizmo.h"

class Framebuffer;

class Viewport
{
public:
	Viewport();

	~Viewport();

	void Draw(Framebuffer* framebuffer, Framebuffer* gameBuffer);

	inline float4 GetBounds() { return bounds; }
	inline const bool GetState() const { return selected; }

	inline bool IsFocused() { return isFocused; }
	void SetSnap(ImGuizmo::OPERATION operation);

	void SnapOptions();

private:
	float2 sizeViewport;
	float4 bounds;
	bool active;

	bool selected;
	bool isFocused;

	bool translateSnap = false;
	bool rotateSnap = false;
	bool scaleSnap = false;

	float allTsnap = 1.0f;
	int allRsnap = 15;
	float allSsnap = 0.5f;

	float snap[3] = { 0,0,0 };
	ImGuizmo::OPERATION currentOperation = ImGuizmo::OPERATION::TRANSLATE;
};