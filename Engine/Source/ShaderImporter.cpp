#include "ShaderImporter.h"

#include "ResourceManager.h"

#include "Resource.h"
#include "Shader.h"

#include "Globals.h"

#include <fstream>

uint ShaderImporter::SaveShader(std::string& assets, std::string& source)
{
	std::string lib;
	uint uid = ResourceManager::GetInstance()->CreateResource(ResourceType::SHADER, assets, lib);
	CopyFileA(assets.c_str(), lib.c_str(), false);

	return uid;
}
