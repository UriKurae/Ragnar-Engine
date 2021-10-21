#include "TextureBuffer.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"

#include "glew/include/GL/glew.h"
#include "IL/il.h"

#include "mmgr/mmgr.h"

TextureBuffer::TextureBuffer(MaterialComponent* material) : id(material->GetId())
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, material->GetWidth(), material->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
}

TextureBuffer::~TextureBuffer()
{
	glDeleteTextures(1, &id);
}

void TextureBuffer::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void TextureBuffer::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}