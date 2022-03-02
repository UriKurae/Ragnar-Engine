#pragma once

#include "Component.h"
#include "Color.h"

class ImageComponent : public Component
{
public:

	ImageComponent(int id, std::string text);
	~ImageComponent();

	bool Update(float dt) override;
	void Draw() override;
	void OnEditor() override;

	State state;
	Color color;
	std::string text;
};