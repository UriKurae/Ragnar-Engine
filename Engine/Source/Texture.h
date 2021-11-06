#pragma once

#include <string>

class Texture
{
public:
	Texture(unsigned int& i, int& w, int& h, std::string& p);

	~Texture();

	void Bind();
	void Unbind();

private:
	unsigned int id;
	int width;
	int height;

	std::string path;
};