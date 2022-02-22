#pragma once

#include "Framebuffer.h"
#include "MathGeoLib/src/Math/float2.h"
#include "MathGeoLib/src/Math/float4.h"


class Viewport
{
public:
	Viewport();

	~Viewport();

	void Draw(Framebuffer* framebuffer, Framebuffer* gameBuffer, int currentOperation);

	inline float4 GetBounds() { return bounds; }
	inline const bool GetState() const { return selected; }
	float4 bounds;

private:
	float2 sizeViewport;
	
	bool active;

	bool selected;
};