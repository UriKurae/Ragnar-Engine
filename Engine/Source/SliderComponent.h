#pragma once

#include "Component.h"
#include "Color.h"
#include "Text.h"
class MyPlane;
class SDL_Rect;
class SDL_Color;
class SDL_Texture;

class SliderComponent : public Component
{
public:

	SliderComponent(GameObject* own);
	virtual ~SliderComponent();

	bool Update(float dt) override;
	void Draw(CameraComponent* gameCam = nullptr) override;
	void OnEditor() override;
	float2 GetParentPosition();
	GameObject* gen;
	float value;
	float minValue;
	float maxValue;
	
	bool completed;
	float barProgres;
public:
	MyPlane* thePlane;
	State state;
	Color actualColor;
	Color disabledColor = white;
	Color pressedColor = red;
	Color focusedColor = blue;
	Color normalColor = green;
	Color selectedColor = black;

	bool drawRect;

	Text sliderText;
	Color textColor = white;
	char text[64] = "Camera FOV";
	float fontScale = 0.5;
};