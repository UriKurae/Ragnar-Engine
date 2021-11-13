#pragma once

#include <string>
#include <assimp/material.h>
#include "JsonParsing.h"

class Texture;

namespace TextureImporter
{
	void ImportTexture(aiMaterial* material, aiTextureType type, const char* typeName, JsonParsing& json);
	void ImportTexture(std::string& fileName);
	void SaveTexture(std::string& fileName);
	Texture* LoadTexture(const char* path);
}