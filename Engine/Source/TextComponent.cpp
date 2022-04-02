#include "TextComponent.h"
#include "Application.h"

#include "ModuleInput.h"
#include "ModuleUI.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "Transform2DComponent.h"

#include "GL/glew.h"

TextComponent::TextComponent(GameObject* own)
{
	type = ComponentType::UI_TEXT;
	own->isUI = true;
	active = true;
	textToShow.setText("Button", 5, 5, 0.5, { 255,255,255 });
	
	state = State::NORMAL;
	actualColor = normalColor;

	if (!own->GetComponent<ComponentTransform2D>()) // If comes from Load not enter
	{
		own->CreateComponent(ComponentType::TRANFORM2D);
		own->CreateComponent(ComponentType::MATERIAL);
		app->userInterface->UIGameObjects.push_back(own);
	}	
	app->userInterface->OrderButtons();
	//planeToDraw = new MyPlane(float3{ 0,0,0 }, float3{ 1,1,1 });
	//planeToDraw->own = own;
}

TextComponent::~TextComponent()
{
}

bool TextComponent::Update(float dt)
{
	textToShow.SetOnlyPosition(float2(GetParentPosition().x, GetParentPosition().y));
	textToShow.setOnlyText(text);
	if (!active)
		state = State::DISABLED;
	else
		state = State::NORMAL;

	return true;
}

void TextComponent::Draw(CameraComponent* gameCam)
{
}

void TextComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("TextComponent"))
	{
		static float multiplier = 1;
		static float fadeDuration = 0.1f;
		static bool textColorEditable = false;
		Checkbox(this, "Active", active);
		ImGui::Text("Text Color"); ImGui::SameLine();
		if (ImGui::ColorButton("Text Color", ImVec4(textColor.r, textColor.g, textColor.b, textColor.a)))
			textColorEditable = !textColorEditable;

		if (textColorEditable)
			ImGui::ColorPicker3("Text Color", &textColor);

		buttonText.setOnlyColor({ textColor.r, textColor.g, textColor.b });

		ImGui::SliderFloat("Color Multiplier", &multiplier, 1, 5);
		ImGui::InputFloat("Fade Duration", &fadeDuration);

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
	return { position.x - (strlen(text) * 12 * textToShow.Scale), position.y - 5 };
}
bool TextComponent::OnLoad(JsonParsing& node)
{
	std::string aux = node.GetJsonString("buttonText");
	strcpy(text, aux.c_str());
	textToShow.textt = text;

	fontScale = node.GetJsonNumber("fontScale");
	textColor.r = node.GetJsonNumber("textColor.r");
	textColor.g = node.GetJsonNumber("textColor.g");
	textColor.b = node.GetJsonNumber("textColor.b");

	return true;
}

bool TextComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "buttonText", text);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "fontScale", fontScale);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.r", textColor.r);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.g", textColor.g);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.b", textColor.b);
	node.SetValueToArray(array, file.GetRootValue());

	return true;
}