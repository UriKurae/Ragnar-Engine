#include "Texture.h"
#include "DevIL/include/IL/il.h"

#include "glew/include/GL/glew.h"

// TODO: Change uid from Resource constructor
Texture::Texture(unsigned int& i, int& w, int& h, GLubyte* d, std::string& p) 
	: id(i), width(w), height(h), path(p), data(d), Resource(0, ResourceType::TEXTURE)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}