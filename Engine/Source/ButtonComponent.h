#pragma once
#include "Component.h"
#include "Color.h"
#include "Text.h"

class MyPlane;
class GameObject;
class MaterialComponent;
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
	State GetState() { return state; };
	inline Text GetButtonText() { return buttonText; };
	inline char* GetText() { return text; };
	void SetText(char* newText) 
	{ 
		buttonText.setOnlyText(newText);
		strcpy(text, newText);
	}

	inline Color GetActualColor() { return actualColor; };
	inline Color GetTextColor() { return textColor; };

	inline void SetNormalMaterial(MaterialComponent* texture) { normalMaterial = texture; };
	inline void SetFocusedMaterial(MaterialComponent* texture) { focusedMaterial = texture; };
	inline void SetPressedMaterial(MaterialComponent* texture) { pressedMaterial = texture; };
	inline void SetDisabledMaterial(MaterialComponent* texture) { disabledMaterial = texture; };
	inline void SetActualMaterial(MaterialComponent* texture) { actual = texture; };

	inline void SetTextPosition(float3 newPoistion) {textPos = newPoistion;}
	inline float3 GetTextPosition() { return textPos; }
	
	inline MaterialComponent* GetNormalMaterial() { return normalMaterial; };
	inline MaterialComponent* GetFocusedMaterial() { return focusedMaterial; };
	inline MaterialComponent* GetPressedMaterial() { return pressedMaterial; };
	inline MaterialComponent* GetDisabledMaterial() { return disabledMaterial; };
	inline MaterialComponent* GetActualMaterial() { return actual; };


	inline void SetAlpha(float Alpha) { alpha = Alpha; };
	inline float GetAlpha() { return alpha; };
	MyPlane* planeToDraw;

private:
	
	float alpha = 1.0f;
	Text buttonText;
	State state = State::NORMAL;
	bool fadeUI = false;
	float3 textPos={0,0,0};
	MaterialComponent* normalMaterial;
	MaterialComponent* focusedMaterial;
	MaterialComponent* pressedMaterial;
	MaterialComponent* disabledMaterial;
	MaterialComponent* actual;

	Color textColor = white;
	Color actualColor = white;

	char text[64] = "Button";
	float fontScale = 1;
};