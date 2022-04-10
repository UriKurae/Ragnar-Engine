#pragma once

#include "Component.h"
#include "Color.h"
#include "Text.h"

class SDL_Rect;
class SDL_Texture;
class MyPlane;
class GameObject;
class TextComponent : public Component
{
public:

	TextComponent(GameObject* own);
	~TextComponent();

	bool Update(float dt) override;
	void Draw(CameraComponent* gameCam = nullptr) override;
	void OnEditor() override;
	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	float2 GetParentPosition();

	char* GetText() 
	{ 
		char* rText{};
		strcpy(rText, textToShow.textt.c_str());
		return rText;
	}
	void SetText(char* newText)
	{		
		textToShow.setOnlyText(newText);
		//strcpy(text, newText);
	}
	void setTextColor(float Red, float Green, float Blue);
	float3 GetTextColor();
	Text textToShow;
private:

	void loadtext(std::string path);
	/*void savetext();*/
	bool fadeUI = false;
	Color textColor = white;
	MyPlane* planeToDraw;
	//char text[358] = "Text";
	float fontScale = 1;
	std::string fileText = "-";
};
