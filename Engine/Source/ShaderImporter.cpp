#include "ShaderImporter.h"
#include "Globals.h"

#include "ResourceManager.h"
#include "Resource.h"

uint ShaderImporter::SaveShader(std::string& assets, std::string& source)
{
	std::string lib;
	uint uid = ResourceManager::GetInstance()->CreateResource(ResourceType::SHADER, assets, lib);
	bool failed = false;
	CopyFileA(assets.c_str(), lib.c_str(), failed);

	if (failed) DEBUG_LOG("Failed to save the shader %s into Library", assets.c_str());

	return uid;
}
