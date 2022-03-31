#pragma once

typedef unsigned int GLuint;

class Framebuffer
{
public:
	Framebuffer(int w, int h, int channel);
	~Framebuffer();

	void Bind();
	void Unbind();

	void SetFramebuffer();
	void ResizeFramebuffer(int w, int h);

	inline const GLuint GetId() const { return framebuffer; }
	inline const GLuint GetColorId() const { return colorTexture; }
	inline const GLuint GetNormalId() const { return normalTexture; }
private:
	int width;
	int height;
	int channelId;

	GLuint framebuffer;
	GLuint rboDepthStencil;
	GLuint colorTexture;
	GLuint normalTexture;
};