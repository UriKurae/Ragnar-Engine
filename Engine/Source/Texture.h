#pragma once

#include "Resource.h"
#include <string>

typedef unsigned char GLubyte;

class Texture : public Resource
{
public:
	Texture(uint uid) : Resource(uid, ResourceType::TEXTURE) {}
	Texture(unsigned int& i, int& w, int& h, GLubyte* d, std::string& p);

	~Texture();

	void Bind();
	void Unbind();

	inline const std::string& GetPath() const { return path; }
	inline const int& GetWidth() const { return width; }
	inline const int& GetId() const { return id; }
	inline const int& GetHeight() const { return height; }

private:
	unsigned int id;
	int width;
	int height;

	std::string path;
	GLubyte* data;
};