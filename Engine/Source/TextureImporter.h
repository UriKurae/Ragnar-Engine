#pragma once

#include <string>
#include <assimp/material.h>
#include "JsonParsing.h"

typedef unsigned char GLubyte;
typedef unsigned int uint;

struct TextureParameters;

namespace TextureImporter
{
	void ImportTexture(aiMaterial* material, aiTextureType type, const char* typeName, JsonParsing& json, std::string& path);
	void ImportTexture(std::string& fileName);
	void ImportTexture2(std::string& fileName);
	void SaveTexture(std::string& fileName);
	void LoadTexture(const char* path, unsigned int& id, int& width, int& height, GLubyte* data, TextureParameters& parameterData);
	void CreateMetaTexture(std::string& path, TextureParameters& data, std::string& assets, uint uid);
	void SetIluParameters(TextureParameters& data);
}