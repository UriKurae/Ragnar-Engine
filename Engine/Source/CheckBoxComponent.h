//#pragma once
//
//#include "Component.h"
//#include "Color.h"
//#include "Text.h"
//
//class CheckboxComponent : public Component
//{
//public:
//
//	CheckboxComponent(int id, std::string text);
//	virtual ~CheckboxComponent();
//
//	bool Update(float dt) override;
//	void Draw() override;
//	void OnEditor(int i) override;
//	void OnClick() override;
//	float2 GetParentPosition();
//
//public:
//	Color disabledColor = White;
//	Color pressedColor = Red;
//	Color focusedColor = Blue;
//	Color normalColor = Green;
//	Color selectedColor = Black;
//
//	bool checked;
//	State state;
//	Text checkboxText;
//	Color textColor = White;
//	char text[64] = "V-Sync";
//	float fontScale = 0.5;
//	bool firstTime;
//};