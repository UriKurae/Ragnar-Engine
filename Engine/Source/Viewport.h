#pragma once

#include "Framebuffer.h"
#include "MathGeoLib/src/Math/float2.h"
#include "MathGeoLib/src/Math/float4.h"


class Viewport
{
public:
	Viewport();

	~Viewport();

	void Draw(Framebuffer* framebuffer);

	inline float4 GetBounds() { return bounds; }
	inline bool GetState() { return selected; }

private:
	float2 sizeViewport;
	float4 bounds;

	bool selected;
};