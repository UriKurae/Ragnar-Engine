#pragma once
#include "Math/float2.h"
#include "Math/float4.h"

class Framebuffer;

class GameView
{
public:
	GameView();

	~GameView();

	void Draw(Framebuffer* framebuffer);

	float4 GetBounds();
	inline const bool GetState() const { return selected; }	

	inline bool IsFocused() { return isFocused; }

private:
	float2 sizeViewport;
	float4 bounds;
	bool active;

	bool selected;
	bool isFocused;
};