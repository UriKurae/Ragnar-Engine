#pragma once

struct Texture;

class TextureBuffer
{
public:
	TextureBuffer() : id(0) {}
	TextureBuffer(Texture texture);
	~TextureBuffer();

	void Bind();

	void Unbind();

private:
	unsigned int id;
};