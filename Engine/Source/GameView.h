#pragma once

#include "Framebuffer.h"
#include "MathGeoLib/include/Math/float2.h"
#include "MathGeoLib/include/Math/float4.h"

class GameView
{
public:
	GameView();

	~GameView();

	void Draw(Framebuffer* framebuffer);

	inline float4 GetBounds() { return bounds; }
	inline const bool GetState() const { return selected; }
	float4 bounds;

private:
	float2 sizeViewport;
	
	bool active;

	bool selected;
};