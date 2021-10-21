#pragma once

#include "Component.h"
#include <string>

typedef unsigned int GLuint;
typedef unsigned char GLubyte;

class MaterialComponent : public Component
{
public:
	MaterialComponent();
	~MaterialComponent();

	void OnEditor() override;

	inline void SetWidth(int w) { width = w; }
	inline void SetHeight(int h) { height = h; }
	inline void SetId(int i) { id = i; }
	inline void SetPath(const char* p) { path = p; }

	inline const GLuint GetId() const { return id; }
	inline const int GetWidth() const { return width; }
	inline const int GetHeight() const { return height; }
	inline const std::string GetPath() const { return path; }

private:
	GLuint id;
	std::string path;
	int width;
	int height;
};