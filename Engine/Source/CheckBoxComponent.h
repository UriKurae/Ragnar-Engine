#pragma once

#include "Component.h"
#include "Color.h"
#include "Text.h"
class MyPlane;
class CheckboxComponent : public Component
{
public:

	CheckboxComponent(GameObject* ow);
	virtual ~CheckboxComponent();

	bool Update(float dt) override;
	void Draw() override;
	void OnEditor() override;
	void OnClick();
	float2 GetParentPosition();

public:
	Color disabledColor = white;
	Color pressedColor = red;
	Color focusedColor = blue;
	Color normalColor = green;
	Color selectedColor = black;
	MyPlane* planeToDraw;
	GameObject* gen;
	bool checked;
	State state;
	Text checkboxText;
	Color textColor = white;
	char text[64] = "V-Sync";
	float fontScale = 0.5;
	bool firstTime;
};