#pragma once
#include "Module.h"
#include <map>
#include"GameObject.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include<vector>
#include "GL/glew.h"
#include"ModuleUI.h"
#include"Transform2DComponent.h"
#include"MathGeoLib/include/Geometry/LineSegment.h"
class Texture;
class GameObject;

class Shadert
{
public:
	unsigned int ID;

	Shadert(const std::string& vertex, const std::string& fragment);
	// activate the shader
	// ------------------------------------------------------------------------
	void Use();

	void StopUse();

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void CheckCompileErrors(uint shader, std::string type);

};


class MyPlane
{
public:
	MyPlane(float3 pos, float3 sca);
    ~MyPlane();
    void DrawPlane();
    void DrawPlane2D(Texture* texture);
	std::vector<float3> getVertex() {
	}

public:
    GLuint imageID;
	float3 position;
	float3 scale;
	AABB aabb;
	unsigned int VBO;
	unsigned int VAO;
	//unsigned int EBO;
	unsigned int TBO;
	GLuint aTextureId;
    GameObject* own;
public:
    std::vector<GLushort> indices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;
    std::vector<float3> vertices;

    Shadert* shader;
};
struct IVec2 {
    IVec2() = default;
    IVec2(int x, int y) : x(x), y(y) {}

    int x = 0, y = 0;
};
/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    IVec2  Size;      // Size of glyph
    IVec2  Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};


class ModuleUI : public Module
{
public:
	ModuleUI(bool startEnabled = true);
	~ModuleUI();

	bool Start()override;
    bool PreUpdate(float dt)override;
    bool Update(float dt)override;

	bool CleanUp();
	void RenderText(std::string text, float x, float y, float scale, float3 color);
    void DeleteUIGameObjects();
public:
    std::map<char, Character> characters;
    Shadert* shader{ nullptr }; //TODO:Clean
    uint VAO = 0, VBO = 0;
	LineSegment myRay;
	float3 corners[8];
	std::vector<GameObject*> hitObjs;
	GameObject* focusedGameObject;
    std::string textExample = "Default";

    float3 color = { 255,255,255 };
    float scale = 1;
    std::vector<GameObject*> UIGameObjects;
    GameObject* UIGameObjectSelected;
    uint nextUIGameObject = 1;
    float2 fMousePos;
};