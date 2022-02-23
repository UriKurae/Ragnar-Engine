#pragma once

#include "Component.h"
#include "Color.h"
#include "Text.h"
#include "ModuleUI.h"
class SDL_Rect;
class SDL_Texture;
class MyPlane;
class ButtonComponent : public Component
{
public:

	ButtonComponent(GameObject* own);
	~ButtonComponent();

	bool Update(float dt) override;
	void Draw() override;
	void OnEditor() override;


	float2 GetParentPosition();

public:
	float buttonWidth;
	float buttonHeight;
	GameObject* gen;
	Text buttonText;
	bool fadeUI = false;
	State state;
	Color disabledColor = white;
	
	Color pressedColor = red;
	Color focusedColor = blue;
	Color normalColor = green;
	Color selectedColor = black;
	MyPlane* planeToDraw;
	Color textColor = white;
	char text[64] = "Button";
	float fontScale = 1;
};