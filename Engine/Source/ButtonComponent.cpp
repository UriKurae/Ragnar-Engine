#include "Application.h"
#include "SDL.h"
#include "ButtonComponent.h"
#include"GameObject.h"
#include "ModuleInput.h"
#include "ModuleUI.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"

ButtonComponent::ButtonComponent(GameObject* own)
{
	owner = own;
	active = true;
	//name = "Button Component";
	type = ComponentType::UI_BUTTON;
	state = State::NORMAL;
	buttonText.setText("Button", 5, 5, 0.5, { 255,255,255 });
	actualColor = normalColor;

}

ButtonComponent::~ButtonComponent()
{
}

bool ButtonComponent::Update(float dt)
{
	buttonText.SetOnlyPosition(float2(GetParentPosition().x, GetParentPosition().y));

	if (!active)
		state = State::DISABLED;
	else
		state = State::NORMAL;




	if (state != State::DISABLED)
	{
		
		if (app->userInterface->focusedGameObject == owner)
		{
			state = State::FOCUSED;

			if (state != State::FOCUSED && state != State::PRESSED)
			{
			}

			if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = State::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				
			}
		}
		else state = State::NORMAL;

		if (app->userInterface->UIGameObjectSelected == owner)
		{
			state = State::SELECTED;
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) 
			{

			}
				
		}
	}

	/*if (fadeUI) 
		FadeUI();*/
	return true;
}

void ButtonComponent::Draw(CameraComponent* gameCam)
{
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	switch (state)
	{
	case State::DISABLED:
		glColor4f(disabledColor.r, disabledColor.g, disabledColor.b, disabledColor.a);
		actualColor = disabledColor;
		break;
	case State::NORMAL:
		glColor4f(normalColor.r, normalColor.g, normalColor.b, normalColor.a);
		actualColor = disabledColor;
		break;
	case State::FOCUSED:
		glColor4f(focusedColor.r, focusedColor.g, focusedColor.b, focusedColor.a);
		actualColor = normalColor;
		break;
	case State::PRESSED:
		glColor4f(pressedColor.r, pressedColor.g, pressedColor.b, pressedColor.a);
		actualColor = pressedColor;
		break;
	case State::SELECTED:
		glColor4f(selectedColor.r, selectedColor.g, selectedColor.b, selectedColor.a);
		actualColor = selectedColor;
		break;
	default:
		break;
	}

	MaterialComponent* mat = owner->GetComponent<MaterialComponent>();
	planeToDraw->DrawPlane2D(mat->GetTexture().get());
	
	glDisable(GL_ALPHA_TEST);
	glColor3f(255, 255, 255);
}

void ButtonComponent::OnEditor()
{
	
	if (ImGui::CollapsingHeader("Button"))
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

		ImGui::Checkbox("Interactable", &active);

		ImGui::Text("Normal Color"); ImGui::SameLine();
		if (ImGui::ColorButton("Normal Color", ImVec4(normalColor.r, normalColor.g, normalColor.b, normalColor.a)))
			normalEditable = !normalEditable;

		ImGui::Text("Pressed Color"); ImGui::SameLine();
		if (ImGui::ColorButton("Pressed Color", ImVec4(pressedColor.r, pressedColor.g, pressedColor.b, pressedColor.a)))
			pressedEditable = !pressedEditable;

		ImGui::Text("Focused Color"); ImGui::SameLine();
		if (ImGui::ColorButton("Focused Color", ImVec4(focusedColor.r, focusedColor.g, focusedColor.b, focusedColor.a)))
			focusedEditable = !focusedEditable;

		ImGui::Text("Disabled Color"); ImGui::SameLine();
		if (ImGui::ColorButton("Disabled Color", ImVec4(disabledColor.r, disabledColor.g, disabledColor.b, disabledColor.a)))
			disabledEditable = !disabledEditable;

		ImGui::Text("Selected Color"); ImGui::SameLine();
		if (ImGui::ColorButton("Selected Color", ImVec4(selectedColor.r, selectedColor.g, selectedColor.b, selectedColor.a)))
			selectedEditable = !selectedEditable;

		ImGui::Separator();

		ImGui::Text("Text Color"); ImGui::SameLine();
		if (ImGui::ColorButton("Text Color", ImVec4(textColor.r, textColor.g, textColor.b, textColor.a)))
			textColorEditable = !textColorEditable;

		buttonText.setOnlyColor({ textColor.r, textColor.g, textColor.b });

		if (normalEditable)
		{
			ImGui::ColorPicker3("Normal Color", &normalColor);
		}
		if (pressedEditable)
		{
			ImGui::ColorPicker3("Pressed Color", &pressedColor);
		}
		if (focusedEditable)
		{
			ImGui::ColorPicker3("Focused Color", &focusedColor);
		}
		if (disabledEditable)
		{
			ImGui::ColorPicker3("Disabled Color", &disabledColor);
		}
		if (selectedEditable)
		{
			ImGui::ColorPicker3("Selected Color", &selectedColor);
		}
		if (textColorEditable)
		{
			ImGui::ColorPicker3("Text Color", &textColor);
		}

		ImGui::SliderFloat("Color Multiplier", &multiplier, 1, 5);
		ImGui::InputFloat("Fade Duration", &fadeDuration);

		ImGui::InputText("Text", text, IM_ARRAYSIZE(text));
		ImGui::DragFloat("Font Size", &buttonText.Scale, 0.1, 0, 10);
		buttonText.setOnlyText(text);
		ImGui::Separator();
	}
	// General variables
	

	
}



float2 ButtonComponent::GetParentPosition()
{
	ComponentTransform2D* transform2D =owner->GetComponent<ComponentTransform2D>();
	float3 position = transform2D->position;
	return { position.x - (strlen(text) * 12 * buttonText.Scale), position.y - 5 };
}
bool ButtonComponent::OnLoad(JsonParsing& node)
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

bool ButtonComponent::OnSave(JsonParsing& node, JSON_Array* array)
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

