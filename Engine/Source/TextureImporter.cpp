#include "TextureImporter.h"

#include "Application.h"
#include "FileSystem.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "Component.h"
#include "Texture.h"

#include "IL/il.h"
#include "ResourceManager.h"
#include "glew/include/GL/glew.h"
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
		std::string assetsPath = RESOURCES_FOLDER;
		assetsPath += aux;

		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);
		ilLoadImage(assetsPath.c_str());
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		std::string libraryPath;
		
		ResourceManager::GetInstance()->CreateResource(ResourceType::TEXTURE, assetsPath, libraryPath);

		json.SetNewJsonNumber(json.ValueToObject(json.GetRootValue()), "Type", (int)ComponentType::MATERIAL);
		json.SetNewJsonString(json.ValueToObject(json.GetRootValue()), "Texture Path", libraryPath.c_str());

		SaveTexture(libraryPath);
		ilDeleteImages(1, &image);
	}
}

void TextureImporter::ImportTexture(std::string& fileName)
{
	if (ResourceManager::GetInstance()->CheckResource(fileName))
	{
		return;
	}
	else
	{
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);
		ilLoadImage(fileName.c_str());
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		std::string libraryPath;

		ResourceManager::GetInstance()->CreateResource(ResourceType::TEXTURE, fileName, libraryPath);

		SaveTexture(libraryPath);
		ilDeleteImages(1, &image);
	}
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

void TextureImporter::LoadTexture(const char* path, unsigned int& id, int& width, int& height, GLubyte* data)
{
	char* buffer = nullptr;

	unsigned int size = app->fs->Load(path, &buffer);

	if (size > 0)
	{
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);
		ilLoadL(IL_DDS, buffer, size);
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		width = ilGetInteger(IL_IMAGE_WIDTH);
		height = ilGetInteger(IL_IMAGE_HEIGHT);

		data = ilGetData();

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		ilDeleteImages(1, &image);
	}

	RELEASE_ARRAY(buffer);
}