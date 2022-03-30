#pragma once

typedef unsigned int GLuint;

class Framebuffer
{
public:
	Framebuffer(int w, int h, int channel, bool genNormalTexture = false);
	~Framebuffer();

	void Bind();
	void Unbind();

	void SetFramebuffer();
	void ResizeFramebuffer(int w, int h);

	inline const GLuint GetId() const { return framebuffer; }

private:
	int width;
	int height;
	int channelId;

	GLuint framebuffer;
	GLuint rboDepthStencil;
	GLuint colorTexture;
	GLuint normalTexture;

	// Post-Processing purposes
	bool genNormalTex;
};