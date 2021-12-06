#include "Texture.h"
#include "DevIL/include/IL/il.h"

#include "Application.h"
#include "FileSystem.h"
#include "TextureImporter.h"

#include "glew/include/GL/glew.h"
#include "Globals.h"

#include "Imgui/imgui.h"

#include "Profiling.h"

Texture::Texture(uint uid, std::string& assets, std::string& library) 
	: data(nullptr), width(0), height(0), id(0), Resource(uid, ResourceType::TEXTURE, assets, library), parameters({})
{
	std::string metaConfig = TEXTURES_FOLDER + std::string("texture_") + std::to_string(uid) + ".meta";
	TextureImporter::CreateMetaTexture(metaConfig, parameters, assets, uid);
	name = assets;
	app->fs->GetFilenameWithExtension(name);
}

Texture::Texture(uint uid, std::string& library) 
	: data(nullptr), width(0), height(0), id(0), Resource(uid, ResourceType::TEXTURE, std::string(""), library), parameters({})
{
}

Texture::~Texture()
{
}

void Texture::Load()
{
	if (id == 0)
	{
		TextureImporter::LoadTexture(libraryPath.c_str(), id,  width, height, data, parameters);
	}
}

void Texture::UnLoad()
{
	if (id != 0)
	{
		glDeleteTextures(1, &id);
		id = 0;
		width = 0;
		height = 0;
		RELEASE_ARRAY(data);
	}
}

void Texture::DrawOnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Texture Import Settings"))
	{
		ImGui::Checkbox("Alienify", &parameters.alienify);
		ImGui::DragFloat("Blur Avg", &parameters.blurAvg);
		ImGui::DragFloat("Blur Gaussian", &parameters.blurGaussian);
		ImGui::Checkbox("Edge Detect P", &parameters.edgeDetectP);
		ImGui::Checkbox("Edge Detect S", &parameters.edgeDetectS);
		ImGui::Checkbox("Emboss", &parameters.emboss);
		ImGui::DragFloat("Gamma Correct Curve", &parameters.gammaCorrectCurve);
		ImGui::Checkbox("Negative", &parameters.negative);
		ImGui::DragFloat("Noise", &parameters.noise);
		if (ImGui::Button("Apply changes")) Reimport();
		ImGui::DragInt("Pixelization", &parameters.pixelization);
	}
	ImGui::PopID();

}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Reimport()
{
	if (id != 0) glDeleteTextures(1, &id);

	width = 0;
	height = 0;

	std::string metaConfig = TEXTURES_FOLDER + std::string("texture_") + std::to_string(uid) + ".meta";
	TextureImporter::CreateMetaTexture(metaConfig, parameters, assetsPath, uid);
	TextureImporter::LoadTexture(libraryPath.c_str(), id, width, height, data, parameters);
}