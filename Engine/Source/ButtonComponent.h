#pragma once
#include "Component.h"
#include "Color.h"
#include "Text.h"

class MyPlane;
class GameObject;

class ButtonComponent : public Component
{
public:
	ButtonComponent(GameObject* own);
	~ButtonComponent();

	bool Update(float dt) override;
	void Draw(CameraComponent* gameCam = nullptr) override;
	void OnEditor() override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	float2 GetParentPosition();
	inline Text GetButtonText() { return buttonText; };
	inline char* GetText() { return text; };

	inline Color GetActualColor() { return actualColor; };
	inline Color GetTextColor() { return textColor; };

	MyPlane* planeToDraw;
	
private:
	Text buttonText;
	State state = State::NORMAL;
	bool fadeUI = false;

	Color disabledColor = white;
	Color pressedColor = red;
	Color focusedColor = blue;
	Color normalColor = green;
	Color selectedColor = black;
	Color textColor = white;
	Color actualColor = normalColor;

	char text[64] = "Button";
	float fontScale = 1;
};