#pragma once

#include "Component.h"
#include <string>

typedef unsigned int GLuint;
typedef unsigned char GLubyte;

class Texture;

class MaterialComponent : public Component
{
public:
	MaterialComponent(GameObject* own);
	~MaterialComponent();

	void OnEditor() override;
	void SetNewMaterial(int i, int w, int h, std::string& p);

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void BindTexture();
	void UnbindTexture();

	void SetTexture(Texture* tex) { diffuse = tex; }

	inline Texture* GetTexture() const { return diffuse; }

private:
	bool checker;

	Texture* checkerImage;

	Texture* diffuse;
};