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
	~MaterialComponent();

	void OnEditor() override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void BindTexture();
	void UnbindTexture();

	void SetTexture(std::shared_ptr<Resource> tex);

	// TODO: Check because fucking no one use this shit
	//inline Texture* GetTexture() const { return diffuse; }

private:
	bool checker;
	bool showTexMenu;

	Texture* checkerImage;

	std::shared_ptr<Texture> diff;
};