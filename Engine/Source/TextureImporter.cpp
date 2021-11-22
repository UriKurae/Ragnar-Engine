#include "TextureImporter.h"

#include "Application.h"
#include "FileSystem.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "Component.h"
#include "Texture.h"

#include "IL/il.h"
#include "ResourceManager.h"
#include "MathGeoLib/src/Algorithm/Random/LCG.h"

#include "Profiling.h"

void TextureImporter::ImportTexture(aiMaterial* material, aiTextureType type, const char* typeName, JsonParsing& json, std::string& path)
{
	for (unsigned int i = 0; i < material->GetTextureCount(type); ++i)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		std::string aux = str.C_Str();
		app->fs->GetFilenameWithExtension(aux);
		std::string libraryPath = RESOURCES_FOLDER;
		libraryPath += aux;

		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);
		ilLoadImage(libraryPath.c_str());
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		LCG random;
		uint number = random.IntFast();
		app->fs->GetFilenameWithoutExtension(libraryPath);
		libraryPath = TEXTURES_FOLDER + std::to_string(number) + ".dds";
		
		std::shared_ptr<Resource> res = ResourceManager::GetInstance()->CreateResource(ResourceType::TEXTURE, number);
		res->SetPaths(path, libraryPath);

		json.SetNewJsonNumber(json.ValueToObject(json.GetRootValue()), "Type", (int)ComponentType::MATERIAL);
		json.SetNewJsonString(json.ValueToObject(json.GetRootValue()), "Texture Path", libraryPath.c_str());

		SaveTexture(libraryPath);
		ilDeleteImages(1, &image);
	}
}

void TextureImporter::ImportTexture(std::string& fileName)
{
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	ilLoadImage(fileName.c_str());
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	app->fs->GetFilenameWithoutExtension(fileName);
	fileName = TEXTURES_FOLDER + fileName + ".dds";

	SaveTexture(fileName);
	ilDeleteImages(1, &image);
}

void TextureImporter::SaveTexture(std::string& fileName)
{
	ILuint size;
	ILubyte* data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, nullptr, 0);

	if (size > 0)
	{
		data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			if (app->fs->Save(fileName.c_str(), data, size) > 0)
				DEBUG_LOG("Texture saved!");
		}

		RELEASE_ARRAY(data);
	}
}

Texture* TextureImporter::LoadTexture(const char* path)
{
	std::string pathName(path);
	if (pathName.find(".dds") == std::string::npos)
	{
		pathName = TEXTURES_FOLDER + pathName + ".dds";
	}

	Texture* tex = nullptr;
	char* buffer = nullptr;

	unsigned int size = app->fs->Load(pathName.c_str(), &buffer);

	if (size > 0)
	{
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);
		ilLoadL(IL_DDS, buffer, size);
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		int w = ilGetInteger(IL_IMAGE_WIDTH);
		int h = ilGetInteger(IL_IMAGE_HEIGHT);

		GLubyte* data = ilGetData();

		tex = new Texture(image, w, h, data, pathName);
		ResourceManager::GetInstance()->AddTexture(tex);

		ilDeleteImages(1, &image);
	}

	RELEASE_ARRAY(buffer);

	return tex;
}