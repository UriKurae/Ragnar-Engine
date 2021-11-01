#pragma once

class MaterialComponent;
typedef unsigned int GLuint;
typedef unsigned char GLubyte;

class TextureBuffer
{
public:
	TextureBuffer() : id(0) {}
	TextureBuffer(GLuint i, int width, int height);
	TextureBuffer(GLuint i, int width, int height, GLubyte* data);
	~TextureBuffer();

	void Bind();
	void Unbind();

	inline const unsigned int GetID() const { return id; }

private:
	unsigned int id;
};