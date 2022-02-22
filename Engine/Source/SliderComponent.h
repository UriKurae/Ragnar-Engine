//#pragma once
//
//#include "Component.h"
//#include "Color.h"
//#include "Text.h"
//
//class SDL_Rect;
//class SDL_Color;
//class SDL_Texture;
//
//class SliderComponent : public Component
//{
//public:
//
//	SliderComponent(int id, std::string text);
//	virtual ~SliderComponent();
//
//	bool Update(float dt) override;
//	void Draw() override;
//	void OnEditor() override;
//	float2 GetParentPosition();
//	GameObject* gen;
//	float value;
//	float minValue;
//	float maxValue;
//	MyPlane* thePlane;
//	bool completed;
//	float barProgres;
//public:
//	State state;
//	Color disabledColor = white;
//	Color pressedColor = red;
//	Color focusedColor = blue;
//	Color normalColor = green;
//	Color selectedColor = black;
//
//	bool drawRect;
//
//	Text sliderText;
//	Color textColor = white;
//	char text[64] = "Camera FOV";
//	float fontScale = 0.5;
//};