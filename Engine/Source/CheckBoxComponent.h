#pragma once

#include "Component.h"
#include "Color.h"
#include "Text.h"
#include"MaterialComponent.h"
#include"Material.h"
class MyPlane;
class GameObject;
class CheckboxComponent : public Component
{
public:

	CheckboxComponent(GameObject* ow);
	virtual ~CheckboxComponent();

	bool Update(float dt) override;
	void Draw(CameraComponent* gameCam = nullptr) override;
	void OnEditor() override;
	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	float2 GetParentPosition();

public:
	MaterialComponent* noSelectedMaterial;
	MaterialComponent* selectedMaterial;
	MaterialComponent* actual;
	Color actualColor;
	Color disabledColor = white;
	Color pressedColor = white;//red
	Color focusedColor = white;//green
	Color normalColor = white;
	Color selectedColor = white;//black
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