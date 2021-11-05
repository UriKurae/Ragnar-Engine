#include "TextureLoader.h"
#include "MaterialComponent.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "GameObject.h"
#include "FileSystem.h"

#include "IL/il.h"
#include "glew/include/GL/glew.h"

#include "Profiling.h"

TextureLoader* TextureLoader::instance = nullptr;

TextureLoader* TextureLoader::GetInstance()
{
	if (instance == nullptr) instance = new TextureLoader();
	
	return instance;
}

void TextureLoader::ReleaseInstance()
{
	RELEASE(instance);
}

void TextureLoader::ImportTexture(const aiMaterial* material, aiTextureType type, const char* typeName)
{
	for (unsigned int i = 0; i < 1; ++i)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		std::string aux = str.C_Str();
		aux = aux.substr(aux.find_last_of("\\") + 1, aux.length());
		std::string path = ASSETS_FOLDER;
		path += aux;

		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);
		ilLoadImage(path.c_str());
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		int w = ilGetInteger(IL_IMAGE_WIDTH);
		int h = ilGetInteger(IL_IMAGE_HEIGHT);

		path = path.substr(path.find_last_of("/") + 1, path.length());
		path = path.substr(0, path.find_last_of("."));
		path = path.insert(0, LIBRARY_FOLDER MATERIALS_FOLDER);
		path += ".dds";
		//*component = new MaterialComponent(image, w, h, path);
		Uint64 size = SaveTexture(path);
	}
}

Uint64 TextureLoader::SaveTexture(std::string& fileName)
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

	return size;
}

MaterialComponent* TextureLoader::LoadTexture(std::string& path)
{
	char* buffer = nullptr;

	path = path.substr(path.find_last_of("\\") + 1, path.length());
	path = path.substr(0, path.find_last_of("."));

	std::string p = LIBRARY_FOLDER MATERIALS_FOLDER + path;
	p += ".dds";

	unsigned int size = app->fs->Load(p.c_str(), &buffer);

	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	ilLoadL(IL_DDS, buffer, size);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	int w = ilGetInteger(IL_IMAGE_WIDTH);
	int h = ilGetInteger(IL_IMAGE_HEIGHT);

	return new MaterialComponent(image, w, h, p);
}

void TextureLoader::LoadTextureToSelected(std::string& path)
{
	if (app->editor->GetSelected() != nullptr)
	{
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);
		ilLoadImage(path.c_str());
		//ilLoadImage(str.C_Str());
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		int w = ilGetInteger(IL_IMAGE_WIDTH);
		int h = ilGetInteger(IL_IMAGE_HEIGHT);

		if (app->editor->GetSelected()->GetComponent<MaterialComponent>() != nullptr)
		{
			app->editor->GetSelected()->GetComponent<MaterialComponent>()->SetNewMaterial(image, w, h, path);
		}
		else
		{
			MaterialComponent* material = new MaterialComponent(image, w, h, path);
			app->editor->GetSelected()->GetComponent<MeshComponent>()->SetMaterial(material);
			app->editor->GetSelected()->AddComponent(material);
		}
	}
	else
	{
		DEBUG_LOG("There's no game object selected");
	}
}