#pragma once

class MaterialComponent;

class TextureBuffer
{
public:
	TextureBuffer() : id(0) {}
	TextureBuffer(MaterialComponent* material);
	~TextureBuffer();

	void Bind();

	void Unbind();

private:
	unsigned int id;
};