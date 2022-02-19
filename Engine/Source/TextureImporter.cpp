#include "TextureImporter.h"

#include "Application.h"
#include "FileSystem.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "Component.h"
#include "Texture.h"

#include "IL/il.h"
#include "IL/ilu.h"
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
		
		uint id = ResourceManager::GetInstance()->CreateResource(ResourceType::TEXTURE, assetsPath, libraryPath);
		
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

void TextureImporter::ImportTexture2(std::string& fileName)
{
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	ilLoadImage(fileName.c_str());
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	app->fs->GetFilenameWithoutExtension(fileName);
	std::string libraryPath = SETTINGS_FOLDER + std::string("EngineResources/") + fileName + ".rgtexture";

	SaveTexture(libraryPath);
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

void TextureImporter::LoadTexture(const char* path, unsigned int& id, int& width, int& height, GLubyte* data, TextureParameters& parameterData)
{
	std::string aux = path;
	aux = aux.substr(0, aux.find_last_of(".")) + ".meta";
	char* bufferMeta = nullptr;
	uint sizeMeta = app->fs->Load(aux.c_str(), &bufferMeta);
	JsonParsing metaFile(bufferMeta);
	
	// Ilu parameters for the texture
	if (sizeMeta > 0)
	{
		parameterData.alienify = metaFile.GetJsonBool("Alienify");
		parameterData.blurAvg = (uint)metaFile.GetJsonNumber("BlurAvg");
		parameterData.blurGaussian = (uint)metaFile.GetJsonNumber("BlurGaussian");
		parameterData.edgeDetectP = metaFile.GetJsonBool("EdgeDetectP");
		parameterData.edgeDetectS = metaFile.GetJsonBool("EdgeDetectS");
		parameterData.emboss = metaFile.GetJsonBool("Emboss");
		parameterData.gammaCorrectCurve = metaFile.GetJsonNumber("GammaCorrectCurve");
		parameterData.negative = metaFile.GetJsonBool("Negative");
		parameterData.noise = metaFile.GetJsonNumber("Noise");
		parameterData.pixelization = (uint)metaFile.GetJsonNumber("Pixelization");
	}

	bool mipMap = metaFile.GetJsonBool("MipMap");
	uint uuid = metaFile.GetJsonNumber("Uuid");

	char* buffer = nullptr;

	unsigned int size = app->fs->Load(path, &buffer);

	if (size > 0)
	{
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);
		ilLoadL(IL_DDS, buffer, size);
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (sizeMeta > 0) SetIluParameters(parameterData);

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
	RELEASE_ARRAY(bufferMeta);
}

void TextureImporter::CreateMetaTexture(std::string& path, TextureParameters& data, std::string& assets, uint uid)
{
	JsonParsing metaTexture;

	metaTexture.SetNewJsonBool(metaTexture.ValueToObject(metaTexture.GetRootValue()), "Alienify", data.alienify);
	metaTexture.SetNewJsonNumber(metaTexture.ValueToObject(metaTexture.GetRootValue()), "BlurAvg", data.blurAvg);
	metaTexture.SetNewJsonNumber(metaTexture.ValueToObject(metaTexture.GetRootValue()), "BlurGaussian", data.blurGaussian);
	metaTexture.SetNewJsonBool(metaTexture.ValueToObject(metaTexture.GetRootValue()), "EdgeDetectP", data.edgeDetectP);
	metaTexture.SetNewJsonBool(metaTexture.ValueToObject(metaTexture.GetRootValue()), "EdgeDetectS", data.edgeDetectS);
	metaTexture.SetNewJsonBool(metaTexture.ValueToObject(metaTexture.GetRootValue()), "Emboss", data.emboss);
	metaTexture.SetNewJsonNumber(metaTexture.ValueToObject(metaTexture.GetRootValue()), "GammaCorrectCurve", data.gammaCorrectCurve);
	metaTexture.SetNewJsonBool(metaTexture.ValueToObject(metaTexture.GetRootValue()), "Negative", data.negative);
	metaTexture.SetNewJsonNumber(metaTexture.ValueToObject(metaTexture.GetRootValue()), "Noise", data.noise);
	metaTexture.SetNewJsonNumber(metaTexture.ValueToObject(metaTexture.GetRootValue()), "Pixelization", data.pixelization);

	metaTexture.SetNewJsonBool(metaTexture.ValueToObject(metaTexture.GetRootValue()), "MipMap", false);
	metaTexture.SetNewJsonNumber(metaTexture.ValueToObject(metaTexture.GetRootValue()), "Uuid", uid);
	metaTexture.SetNewJsonString(metaTexture.ValueToObject(metaTexture.GetRootValue()), "Assets Path", assets.c_str());

	char* buffer = nullptr;
	size_t size = metaTexture.Save(&buffer);

	app->fs->Save(path.c_str(), buffer, size);

	RELEASE_ARRAY(buffer);
}

void TextureImporter::SetIluParameters(TextureParameters& data)
{

	// Activate parameters 

	if (data.alienify) iluAlienify();
	if (data.blurAvg != 0) iluBlurAvg(data.blurAvg);
	if (data.blurGaussian != 0) iluBlurAvg(data.blurGaussian);
	if (data.edgeDetectP != 0) iluEdgeDetectP();
	if (data.edgeDetectS != 0) iluEdgeDetectS();
	if (data.emboss) iluEmboss();
	if (data.gammaCorrectCurve != 0.0f) iluGammaCorrect(data.gammaCorrectCurve);
	if (data.negative) iluNegative();
	if (data.noise != 0.0f) iluNoisify(data.noise);
	if (data.pixelization != 0) iluPixelize(data.pixelization);
}