#pragma once

#include "JsonParsing.h"
#include "ShaderHelpers.h"

#include <string>

namespace ShaderImporter
{
	unsigned int SaveShader(std::string& assets, std::string& source = std::string());
}