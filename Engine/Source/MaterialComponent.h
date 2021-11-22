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
	void SetNewMaterial(int i, int w, int h, std::string& p);

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void BindTexture();
	void UnbindTexture();

	void SetTexture(Texture* tex) { diffuse = tex; }
	//void SetTexture(std::shared_ptr<Resource> tex) { diff = std::dynamic_pointer_cast<Texture>(tex); }

	// TODO: Check because fucking no one use this shit
	//inline Texture* GetTexture() const { return diffuse; }

private:
	bool checker;

	Texture* checkerImage;

	Texture* diffuse;
	std::shared_ptr<Texture> diff;
};