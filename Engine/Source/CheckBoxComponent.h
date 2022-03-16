#pragma once
#include "Component.h"
#include "Color.h"
#include "Text.h"

#include"Material.h"

class MyPlane;
class GameObject;
class MaterialComponent;

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
	inline Color GetActualColor() { return actualColor; };
	inline Color GetTextColor() { return textColor; };
	inline char* GetText() { return text; };

	inline MaterialComponent* GetNoSelectedMaterial() { return noSelectedMaterial; };
	inline MaterialComponent* GetSelectedMaterial() { return selectedMaterial; };
	inline MaterialComponent* GetActual() { return actual; };

	inline void SetNoSelectedMaterial(MaterialComponent* mat) { noSelectedMaterial = mat; };
	inline void SetSelectedMaterial(MaterialComponent* mat) { selectedMaterial = mat; };
	inline void SetActual(MaterialComponent* mat) { actual = mat; };

	MyPlane* planeToDraw;
private:
	MaterialComponent* noSelectedMaterial;
	MaterialComponent* selectedMaterial;
	MaterialComponent* actual;

	Color disabledColor = white;
	Color pressedColor = white;//red
	Color focusedColor = white;//green
	Color normalColor = white;
	Color selectedColor = white;//black
	Color textColor = white;
	Color actualColor = normalColor;

	State state = State::NORMAL;
	Text checkboxText;

	char text[32] = "V-Sync";
	float fontScale = 0.5;
	bool checked = false;
};