#pragma once

#include "Component.h"
#include "TextureBuffer.h"
#include <string>

typedef unsigned int GLuint;
typedef unsigned char GLubyte;

class MaterialComponent : public Component
{
public:
	MaterialComponent(GameObject* own);
	MaterialComponent(int i, int w, int h, std::string& p);
	MaterialComponent(int i, int w, int h, GLubyte* data);
	~MaterialComponent();

	void OnEditor() override;
	void SetNewMaterial(int i, int w, int h, std::string& p);

	bool OnLoad(JsonParsing& node, JSON_Array* array) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void BindTexture();
	void UnbindTexture();

	void CreateChecker();

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

	bool checker;

	static TextureBuffer* checkerBuffer;
	TextureBuffer* texBuffer;
	GLubyte checkerImage[64][64][4];
};