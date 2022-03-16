#pragma once
#include "Component.h"
#include "Color.h"
#include "Text.h"

class MyPlane;
class GameObject;

class SliderComponent : public Component
{
public:
	SliderComponent(GameObject* own);
	virtual ~SliderComponent();

	bool Update(float dt) override;
	void Draw(CameraComponent* gameCam = nullptr) override;
	void OnEditor() override;

	float2 GetParentPosition();
	inline Color GetActualColor() { return actualColor; };
	inline Color GetTextColor() { return textColor; };
	inline Text GetText() { return sliderText; };

	MyPlane* thePlane;
private:
	float value;
	float minValue;
	float maxValue;
	
	bool completed;
	float barProgres;

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