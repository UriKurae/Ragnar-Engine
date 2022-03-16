#pragma once
#include "Component.h"
#include "Color.h"

class MyPlane;
class GameObject;

class ImageComponent : public Component
{
public:
	ImageComponent(GameObject* own);
	~ImageComponent();

	bool Update(float dt) override;
	void Draw(CameraComponent* gameCam = nullptr) override;
	void OnEditor() override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	inline Color GetColor() { return color; };
	inline Color GetActualColor() { return actualColor; };
	inline std::string GetText() { return text; };

	MyPlane* planeToDraw;
private:
	State state = State::NORMAL;
	Color color = white;
	Color actualColor = color;
	std::string text;
};