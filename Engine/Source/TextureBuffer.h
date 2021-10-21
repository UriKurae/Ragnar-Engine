#pragma once

class MaterialComponent;
typedef unsigned int GLuint;

class TextureBuffer
{
public:
	TextureBuffer() : id(0) {}
	TextureBuffer(GLuint i, int width, int height);
	~TextureBuffer();

	void Bind();

	void Unbind();

private:
	unsigned int id;
};