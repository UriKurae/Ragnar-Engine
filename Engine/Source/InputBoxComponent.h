//#pragma once
//
//#include "SDL.h"
//#include "Component.h"
//#include "Color.h"
//#include "Text.h"
//
//class InputBoxComponent : public Component
//{
//public:
//
//	InputBoxComponent(int id, std::string text);
//	virtual ~InputBoxComponent();
//
//	void Update() override;
//	void Draw() override;
//	void OnEditor(int i) override;
//	void OnClick() override;
//	float2 GetParentPosition();
//
//public:
//	Text aux;
//	State state;
//	Color disabledColor = White;
//	Color pressedColor = Red;
//	Color focusedColor = Blue;
//	Color normalColor = Green;
//	Color selectedColor = Black;
//
//	Color color = normalColor;
//	Color textColor = White;
//	Color prevColor = normalColor;
//
//	char text[64] = "Introduce your name...";
//	float fontScale = 1;
//	bool drawRect;
//	bool textEditable = false;
//};