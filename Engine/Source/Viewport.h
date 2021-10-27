#pragma once

#include "Framebuffer.h"
#include "MathGeoLib/src/Math/float2.h"

class Viewport
{
public:
	Viewport();

	~Viewport();

	void Draw(Framebuffer* framebuffer);

private:
	float2 sizeViewport;
};