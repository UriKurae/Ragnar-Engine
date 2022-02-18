#pragma once

#include "Component.h"
#include "Shader.h"

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

	void Bind();
	void Unbind();

	void SetTexture(std::shared_ptr<Resource> tex);

private:
	bool checker;
	bool showTexMenu;

	std::shared_ptr<Texture> diff = nullptr;
	Shader* shader;

	// TODO: Temporary, should go in Material.h
	float3 ambientColor;
	float3 diffuseColor;
	float3 specularColor;
	float shininess;

};