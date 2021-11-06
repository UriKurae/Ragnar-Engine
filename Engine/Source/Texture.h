#pragma once

#include <string>

class Texture
{
public:
	Texture(unsigned int& i, int& w, int& h, std::string& p);

	~Texture();

	void Bind();
	void Unbind();

	inline const std::string& GetPath() const { return path; }
	inline const int& GetWidth() const { return width; }
	inline const int& GetHeight() const { return height; }

private:
	unsigned int id;
	int width;
	int height;

	std::string path;
};