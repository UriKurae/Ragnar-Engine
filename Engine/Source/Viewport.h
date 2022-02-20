#pragma once

#include "Framebuffer.h"
#include "MathGeoLib/include/Math/float2.h"
#include "MathGeoLib/include/Math/float4.h"


class Viewport
{
public:
	Viewport();

	~Viewport();

	void Draw(Framebuffer* framebuffer, Framebuffer* gameBuffer, int currentOperation);

	inline float4 GetBounds() { return bounds; }
	inline const bool GetState() const { return selected; }

private:
	float2 sizeViewport;
	float4 bounds;
	bool active;

	bool selected;
};