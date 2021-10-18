#include "TextureBuffer.h"
#include "MeshComponent.h"

#include "glew/include/GL/glew.h"
#include "IL/il.h"

TextureBuffer::TextureBuffer(Texture texture) : id(texture.id)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
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