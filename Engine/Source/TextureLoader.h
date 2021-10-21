#pragma once

#include <string>

class MaterialComponent;

class TextureLoader
{
public:
	static TextureLoader* GetInstance();
	static void ReleaseInstance();

	MaterialComponent* LoadTexture(std::string path);
	void LoadTextureToSelected(std::string path);
private:
	TextureLoader() {}
	TextureLoader(const TextureLoader&) {}
	static TextureLoader* instance;
};