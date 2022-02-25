#include "ShaderImporter.h"

#include "ResourceManager.h"

#include "Resource.h"
#include "Shader.h"

#include "Globals.h"

#include <fstream>

uint ShaderImporter::SaveShader(std::string& assets, std::string& source)
{
	std::fstream assetsFile;
	std::fstream libraryFile;
	std::string s = "";

	assetsFile.open(assets);
	if (source == "")
	{
		char ch;
		while (1)
		{
			assetsFile >> ch;
			s += ch;
			if (assetsFile.eof())
				break;
		}
	}
	//assetsFile >> source;
	assetsFile.close();


	uint uid = ResourceManager::GetInstance()->CreateResource(ResourceType::SHADER, assets, std::string());
	std::shared_ptr<Shader> sh = std::static_pointer_cast<Shader>(ResourceManager::GetInstance()->GetResource(uid));

	//source == "" ? sh->UpdateSourceCode(s) : sh->UpdateSourceCode(source);
	//sh->UpdateSourceCode()
	
	libraryFile.open(sh->GetLibraryPath());
	libraryFile << source;
	libraryFile.close();

	return uid;
}

