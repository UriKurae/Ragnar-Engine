#pragma once
#include "Component.h"
#include "Color.h"
#include "Text.h"

class MyPlane;
class GameObject;
class MaterialComponent;

class SliderComponent : public Component
{
public:
	SliderComponent(GameObject* own);
	virtual ~SliderComponent();

	bool Update(float dt) override;
	void Draw(CameraComponent* gameCam = nullptr) override;
	void OnEditor() override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	float2 GetParentPosition();
	inline Color GetActualColor() { return actualColor; };
	inline Color GetTextColor() { return textColor; };
	inline Text GetText() { return sliderText; };

	inline float GetBarProgress() { return barProgres; };
	inline bool GetFirstDraw() { return firstDraw; };

	inline void SetSecondMaterial(MaterialComponent* mat) { secondMaterial = mat; };

	MyPlane* planeToDraw;
	MyPlane* frontPlaneToDraw;
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
	Color textColor = white;

	bool firstDraw = false;
	bool drawRect;

	MaterialComponent* secondMaterial;
	Text sliderText;
	char text[64] = "Camera FOV";
	float fontScale = 0.5;
};