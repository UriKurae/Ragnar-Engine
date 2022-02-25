#pragma once

#include "Component.h"
#include "Shader.h"
#include "TextEditor.h"

#include <string>
#include <memory>



typedef unsigned int GLuint;
typedef unsigned char GLubyte;

class Texture;
class Resource;
class CameraComponent;

class MaterialComponent : public Component
{
public:
	MaterialComponent(GameObject* own, bool defaultMat = true);
	MaterialComponent(MaterialComponent* mat);
	~MaterialComponent();

	void OnEditor() override;

	std::vector<Uniform> GetShaderUniforms();

	void ShowUniforms();

	bool Update(float dt) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void Bind(CameraComponent* gameCam);
	void Unbind();
	bool IsDefaultMat() const { return defaultMat; }

	void SetTexture(std::shared_ptr<Resource> tex);
	void EditorShader();

private:
	bool checker;
	bool showTexMenu;
	bool showShaderEditor;
	bool showShaderMenu;

	TextEditor editor;
	std::string fileToEdit;

	std::shared_ptr<Shader> shadertoRecompily;

	std::shared_ptr<Texture> diff = nullptr;
	std::shared_ptr<Shader> shader;

	// TODO: Temporary, should go in Material.h
	float3 ambientColor;
	float3 diffuseColor;
	float3 specularColor;
	float shininess;

	float refreshShaderTimer;

	bool defaultMat;

};