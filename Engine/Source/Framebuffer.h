#pragma once

typedef unsigned int GLuint;

class Framebuffer
{
public:
	Framebuffer(int w, int h);

	~Framebuffer();

	void Bind();
	void Unbind();

	void SetFramebuffer();

	void ResizeFramebuffer(int w, int h);

	inline const GLuint GetId() const { return framebuffer; }

private:
	int width;
	int height;

	GLuint framebuffer;
	GLuint rboDepthStencil;
	GLuint textureColorbuffer;
};