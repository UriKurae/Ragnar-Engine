#include "TextComponent.h"
#include "Application.h"

#include "ModuleInput.h"
#include "ModuleUI.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "Transform2DComponent.h"

TextComponent::TextComponent(GameObject* own)
{
	owner = own;
	active = true;
	
	type = ComponentType::UI_TEXT;
	state = State::NORMAL;
	buttonText.setText("Button", 5, 5, 0.5, { 255,255,255 });
	actualColor = normalColor;

}

TextComponent::~TextComponent()
{
}

bool TextComponent::Update(float dt)
{
	buttonText.SetOnlyPosition(float2(GetParentPosition().x, GetParentPosition().y));

	if (!active)
		state = State::DISABLED;
	else
		state = State::NORMAL;

	return true;
}

void TextComponent::Draw(CameraComponent* gameCam)
{
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);



	MaterialComponent* mat = owner->GetComponent<MaterialComponent>();
	planeToDraw->DrawPlane2D(mat->GetTexture().get());

	glDisable(GL_ALPHA_TEST);
	glColor3f(255, 255, 255);
}

void TextComponent::OnEditor()
{

	if (ImGui::CollapsingHeader("Text"))
	{
		static float multiplier = 1;
		static float fadeDuration = 0.1f;

		// Manage if colors are being edited or not
		static bool normalEditable = false;
		static bool pressedEditable = false;
		static bool focusedEditable = false;
		static bool disabledEditable = false;
		static bool selectedEditable = false;
		static bool textColorEditable = false;


		Checkbox(this, "Active", active);

		

		

		ImGui::Text("Text Color"); ImGui::SameLine();
		if (ImGui::ColorButton("Text Color", ImVec4(textColor.r, textColor.g, textColor.b, textColor.a)))
			textColorEditable = !textColorEditable;

		buttonText.setOnlyColor({ textColor.r, textColor.g, textColor.b });

		

		ImGui::SliderFloat("Color Multiplier", &multiplier, 1, 5);
		ImGui::InputFloat("Fade Duration", &fadeDuration);

		ImGui::InputText("Text", text, IM_ARRAYSIZE(text));
		ImGui::DragFloat("Font Size", &buttonText.Scale, 0.1, 0, 10);
		buttonText.setOnlyText(text);

		ComponentOptions(this);
		ImGui::Separator();
	}
	// General variables



}

float2 TextComponent::GetParentPosition()
{
	ComponentTransform2D* transform2D = owner->GetComponent<ComponentTransform2D>();
	float3 position = transform2D->GetPosition();
	return { position.x - (strlen(text) * 12 * buttonText.Scale), position.y - 5 };
}
bool TextComponent::OnLoad(JsonParsing& node)
{
	std::string text;

	planeToDraw = new MyPlane(float3{ 0,0,0 }, float3{ 1,1,1 });
	planeToDraw->own = owner;
	owner->isUI = true;
	app->userInterface->UIGameObjects.push_back(owner);
	text = node.GetJsonNumber("buttonText");
	buttonText.textt = text;
	fontScale = node.GetJsonNumber("fontScale");
	textColor.r = node.GetJsonNumber("textColor.r");
	textColor.g = node.GetJsonNumber("textColor.g");
	textColor.b = node.GetJsonNumber("textColor.b");
	return true;
}

bool TextComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();
	std::string text = buttonText.textt;

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "buttonText", text.c_str());
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "fontScale", fontScale);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.r", textColor.r);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.g", textColor.g);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.b", textColor.b);
	node.SetValueToArray(array, file.GetRootValue());

	return true;
}