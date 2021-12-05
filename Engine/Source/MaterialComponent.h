#pragma once

#include "Component.h"
#include <string>
#include <memory>

typedef unsigned int GLuint;
typedef unsigned char GLubyte;

class Texture;
class Resource;

class MaterialComponent : public Component
{
public:
	MaterialComponent(GameObject* own);
	MaterialComponent(MaterialComponent* mat);
	~MaterialComponent();

	void OnEditor() override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void BindTexture();
	void UnbindTexture();

	void SetTexture(std::shared_ptr<Resource> tex);

private:
	bool checker;
	bool showTexMenu;

	std::shared_ptr<Texture> diff;
};