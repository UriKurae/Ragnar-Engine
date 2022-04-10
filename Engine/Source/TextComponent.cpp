#include "TextComponent.h"
#include "Application.h"

#include "ModuleInput.h"
#include "ModuleUI.h"
#include"FileSystem.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "Transform2DComponent.h"
#include "Globals.h"
#include "GL/glew.h"

#include "Profiling.h"

TextComponent::TextComponent(GameObject* own)
{
	type = ComponentType::UI_TEXT;
	own->isUI = true;
	active = true;
	textToShow.setText("Text", 5, 5, 0.5, { 255,255,255 });

	if (!own->GetComponent<ComponentTransform2D>()) // If comes from Load not enter
	{
		own->CreateComponent(ComponentType::TRANFORM2D);
		own->CreateComponent(ComponentType::MATERIAL);
	}	
	app->userInterface->UIGameObjects.push_back(own);
	app->userInterface->OrderButtons();
	//planeToDraw = new MyPlane(float3{ 0,0,0 }, float3{ 1,1,1 });
	//planeToDraw->own = own;

	planeToDraw = new MyPlane(float3{ 0,0,0 }, float3{ 1,1,1 });
	planeToDraw->own = own;
}

TextComponent::~TextComponent()
{
}

bool TextComponent::Update(float dt)
{
	RG_PROFILING_FUNCTION("Text Update");

	textToShow.SetOnlyPosition(float2(GetParentPosition().x, GetParentPosition().y));
	//textToShow.setOnlyText(text);

	return true;
}

void TextComponent::Draw(CameraComponent* gameCam)
{
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	planeToDraw->DrawPlane2D(owner->GetComponent<MaterialComponent>()->GetTexture().get());

	glDisable(GL_ALPHA_TEST);
	glColor3f(255, 255, 255);
}

void TextComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Text"))
	{
		static float multiplier = 1;
		static float fadeDuration = 0.1f;
		static bool textColorEditable = false;

		Checkbox(this, "Active", active);
		char* aux=new char[fileText.size()];
		strcpy(aux, fileText.c_str());
		ImGui::InputText("Text Path", aux, 50);
		fileText = aux;
		delete[] aux;
		ImGui::Text("Text Color"); ImGui::SameLine();
		if (ImGui::ColorButton("Text Color", ImVec4(textColor.r, textColor.g, textColor.b, textColor.a)))
			textColorEditable = !textColorEditable;

		if (textColorEditable)
			ImGui::ColorPicker3("Text Color", &textColor);

		textToShow.setOnlyColor({ textColor.r, textColor.g, textColor.b });

		ImGui::SliderFloat("Color Multiplier", &multiplier, 1, 5);
		ImGui::InputFloat("Fade Duration", &fadeDuration);

		char text[384];
		strcpy(text, textToShow.textt.c_str());
		if(ImGui::InputText("Texte", text, IM_ARRAYSIZE(text)))
			textToShow.setOnlyText(text);
		ImGui::DragFloat("Font Size", &textToShow.Scale, 0.1, 0, 10);

		ComponentOptions(this);
		ImGui::Separator();
	}
}

float2 TextComponent::GetParentPosition()
{
	ComponentTransform2D* transform2D = owner->GetComponent<ComponentTransform2D>();
	float3 position = transform2D->GetPosition();
	return { position.x/* - (textToShow.textt.size() * 12 * textToShow.Scale)*/, position.y/* - 5*/ };
}
bool TextComponent::OnLoad(JsonParsing& node)
{
	std::string aux = node.GetJsonString("buttonText");
	//strcpy(text, aux.c_str());
	

	fontScale = node.GetJsonNumber("fontScale");
	textColor.r = node.GetJsonNumber("textColor.r");
	textColor.g = node.GetJsonNumber("textColor.g");
	textColor.b = node.GetJsonNumber("textColor.b");
	fileText = node.GetJsonString("TextFile");
	if (fileText == "-")
	{
		textToShow.textt = aux;
	}
	else 
	{
		loadtext(fileText);
	}
	


	return true;
}

bool TextComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "buttonText", textToShow.textt.c_str());
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "fontScale", fontScale);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.r", textColor.r);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.g", textColor.g);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.b", textColor.b);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "TextFile", fileText.c_str());
	node.SetValueToArray(array, file.GetRootValue());
	

	return true;
}

void TextComponent::loadtext(std::string path)
{
	JsonParsing dialogFile = JsonParsing();

	if (dialogFile.ParseFile(path.c_str()) > 0)
	{
		
		JSON_Array* jsonArray = dialogFile.GetJsonArray(dialogFile.ValueToObject(dialogFile.GetRootValue()), "AllText");
		JsonParsing c = dialogFile.GetJsonArrayValue(jsonArray, 0);
		std::string aux = c.GetJsonString("Text");

		textToShow.textt = aux;
	}

}
//void TextComponent::savetext()
//{
//	JsonParsing TextFile;
//	JSON_Array* arrayDialogue = TextFile.SetNewJsonArray(TextFile.GetRootValue(), "AllText");
//
//
//	JsonParsing file2 = JsonParsing();
//
//	file2.SetNewJsonString(file2.ValueToObject(file2.GetRootValue()), "Text", "vvvvvvvvv");
//	TextFile.SetValueToArray(arrayDialogue, file2.GetRootValue());
//
//
//	char* buf;
//	uint size = TextFile.Save(&buf);
//
//	std::string savePath = "Assets/Dialogues/";
//	savePath += "Credits";
//	savePath += ".rgdialogue";
//
//	if (app->fs->Save(savePath.c_str(), buf, size) > 0)
//		//DEBUG_LOG("Dialogue saved succesfully");
//	
//		//DEBUG_LOG("Dialogue couldn't be saved");
//
//	RELEASE_ARRAY(buf);
//
//
//}