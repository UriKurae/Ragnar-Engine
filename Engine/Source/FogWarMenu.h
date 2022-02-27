#pragma once
#include "Menu.h"

class FogWarMenu : public Menu
{
public:
	FogWarMenu();
	~FogWarMenu();

	bool Update(float dt) override;

	bool fog = false;
	bool fogLinear = true;
	bool fogExpo = false;
	float fogColor[3] = { 0.8f, 0.8f, 0.8f };
	float fogStart = 10.0f;
	float fogEnd = 40.0f;
	float fogDensity = 1.0f;
	float oldFarPlane = 100.0f;
	float fogFarPlane = 45.0f;
};
