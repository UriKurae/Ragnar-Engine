#pragma once

#include "Component.h"
#include "Color.h"
#include "Text.h"

class SDL_Rect;
class SDL_Texture;
class MyPlane;
class GameObject;
class TextComponent : public Component
{
public:

	TextComponent(GameObject* own);
	~TextComponent();

	bool Update(float dt) override;
	void Draw(CameraComponent* gameCam = nullptr) override;
	void OnEditor() override;
	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	float2 GetParentPosition();

public:
	float buttonWidth;
	float buttonHeight;
	Text buttonText;
	bool fadeUI = false;
	State state;
	Color disabledColor = white;
	Color actualColor;
	Color pressedColor = red;
	Color focusedColor = blue;
	Color normalColor = green;
	Color selectedColor = black;
	MyPlane* planeToDraw;
	Color textColor = white;
	char text[64] = "Button";
	float fontScale = 1;
};
