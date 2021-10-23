#include "TextureLoader.h"
#include "MaterialComponent.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "GameObject.h"

#include "IL/il.h"

#include "mmgr/mmgr.h"

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

MaterialComponent* TextureLoader::LoadTexture(std::string& path)
{
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	ilLoadImage(path.c_str());
	//ilLoadImage(str.C_Str());
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	int w = ilGetInteger(IL_IMAGE_WIDTH);
	int h = ilGetInteger(IL_IMAGE_HEIGHT);

	return new MaterialComponent(image, w, h, path);
}

void TextureLoader::LoadTextureToSelected(std::string& path)
{
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	ilLoadImage(path.c_str());
	//ilLoadImage(str.C_Str());
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	int w = ilGetInteger(IL_IMAGE_WIDTH);
	int h = ilGetInteger(IL_IMAGE_HEIGHT);

	if (app->editor->GetSelected() != nullptr)
	{
		if (app->editor->GetSelected()->GetComponent<MaterialComponent>() != nullptr)
		{
			app->editor->GetSelected()->GetComponent<MaterialComponent>()->SetNewMaterial(image, w, h, path);
		}
		else
		{
			MaterialComponent* material = new MaterialComponent(image, w, h, path);
			app->editor->GetSelected()->AddComponent(material);
		}
	}
	else
	{
		DEBUG_LOG("There's no game object selected");
	}
}