#pragma once

#include <string>
#include "assimp/material.h"

typedef uint64_t Uint64;

class MaterialComponent;

class TextureLoader
{
public:
	static TextureLoader* GetInstance();
	static void ReleaseInstance();

	void ImportTexture(const aiMaterial* material, MaterialComponent** component, aiTextureType type, const char* typeName);
	Uint64 SaveTexture(MaterialComponent* component, char** fileBuffer);
	MaterialComponent* LoadTexture(std::string& path);
	void LoadTextureToSelected(std::string& path);
private:
	TextureLoader() {}
	TextureLoader(const TextureLoader&) {}
	static TextureLoader* instance;
};