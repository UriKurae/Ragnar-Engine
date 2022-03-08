#pragma once

#include "Component.h"
#include "Color.h"
#include "ModuleUI.h"
class SDL_Rect;
class SDL_Texture;
class MyPlane;
class ImageComponent : public Component
{
public:

	ImageComponent(GameObject* own);
	~ImageComponent();
	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;
	bool Update(float dt) override;
	void Draw(CameraComponent* gameCam = nullptr) override;
	void OnEditor() override;
	MyPlane* planeToDraw;
	State state;
	Color color = white;
	Color actualColor;
	GameObject* gen;
	std::string text;
};