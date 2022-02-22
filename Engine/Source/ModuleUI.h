#pragma once
#include "Module.h"
#include <map>
#include"GameObject.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include<vector>
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

class Shader
{
public:
    unsigned int ID;

    Shader();
    // activate the shader
    // ------------------------------------------------------------------------
    void Use();

    void StopUse();

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void CheckCompileErrors(uint shader, std::string type);

};
class ModuleUI : public Module
{
public:
	ModuleUI(bool startEnabled = true);
	~ModuleUI();

	bool Start()override;
    bool PreUpdate(float dt)override;
    bool Update(float dt)override;
    bool PostUpdate()override;

	bool CleanUp();
	void RenderText(std::string text, float x, float y, float scale, float3 color);
    void DeleteUIGameObjects();
public:
    std::map<char, Character> characters;
    Shader* shader{ nullptr }; //TODO:Clean
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