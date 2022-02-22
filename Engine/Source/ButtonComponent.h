#pragma once

#include "Component.h"
#include "Color.h"
#include "Text.h"

class SDL_Rect;
class SDL_Texture;

class ButtonComponent : public Component
{
public:

	ButtonComponent(int id, std::string text);
	~ButtonComponent();

	bool Update(float dt) override;
	void Draw() override;
	void OnEditor() override;


	float2 GetParentPosition();
	void FadeUI();

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

	Color textColor = white;
	char text[64] = "Button";
	float fontScale = 1;
};