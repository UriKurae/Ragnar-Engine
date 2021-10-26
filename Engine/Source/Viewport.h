#pragma once



class Viewport
{
public:
	Viewport();

	~Viewport();

	void Draw(const unsigned int& framebuffer);

private:
	int width;
	int height;
};