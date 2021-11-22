#include "Texture.h"
#include "DevIL/include/IL/il.h"

#include "TextureImporter.h"

#include "glew/include/GL/glew.h"

Texture::Texture(uint uid, std::string& assets, std::string& library) 
	: data(nullptr), width(0), height(0), id(0), Resource(uid, ResourceType::TEXTURE, assets, library)
{
}

Texture::~Texture()
{
}

void Texture::Load()
{
	if (id == 0)
	{
		TextureImporter::LoadTexture(libraryPath.c_str(), id,  width, height, data);
	}
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}