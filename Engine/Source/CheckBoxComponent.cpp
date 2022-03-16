#include "CheckboxComponent.h"
#include "Application.h"
#include "Globals.h"

#include"ModuleUI.h"
#include"ModuleInput.h"

#include"MaterialComponent.h"

CheckboxComponent::CheckboxComponent(GameObject* own)
{
	type = ComponentType::UI_CHECKBOX;
	checkboxText.setText("check", 5, 5, 0.5, { 255,255,255 });	
	own->name = "CheckBox";
}

CheckboxComponent::~CheckboxComponent()
{
	RELEASE(planeToDraw);
}

bool CheckboxComponent::Update(float dt)
{
	checkboxText.SetOnlyPosition(float2(GetParentPosition().x, GetParentPosition().y));

	if (!active)
		state = State::DISABLED;
	else
		state = State::NORMAL;

	if (state != State::DISABLED)
	{
		if (app->userInterface->focusedGameObject == owner)
		{
			state = State::FOCUSED;

			if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = State::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				if (actual == noSelectedMaterial)
					actual = selectedMaterial;
				else 
					actual = noSelectedMaterial;
			}
		}
		else state = State::NORMAL;
	}
	return true;
}

void CheckboxComponent::Draw(CameraComponent* gameCam)
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
		actualColor = normalColor;
		break;
	case State::FOCUSED:
		glColor4f(focusedColor.r, focusedColor.g, focusedColor.b, focusedColor.a);
		actualColor = focusedColor;

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

	planeToDraw->DrawPlane2D(actual->GetTexture().get());

	glDisable(GL_ALPHA_TEST);
	glColor3f(255, 255, 255);
}

void CheckboxComponent::OnEditor()
{
	// General variables
	static float multiplier = 1;
	static float fadeDuration = 0.1f;

	// Manage if colors are being edited or not
	static bool normalEditable = false;
	static bool pressedEditable = false;
	static bool focusedEditable = false;
	static bool disabledEditable = false;
	static bool selectedEditable = false;
	static bool textColorEditable = false;
	if (ImGui::CollapsingHeader("CheckBox"))
	{
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

		if (normalEditable)
			ImGui::ColorPicker3("Normal Color", &normalColor);
		if (pressedEditable)
			ImGui::ColorPicker3("Pressed Color", &pressedColor);
		if (focusedEditable)
			ImGui::ColorPicker3("Focused Color", &focusedColor);
		if (disabledEditable)
			ImGui::ColorPicker3("Disabled Color", &disabledColor);
		if (selectedEditable)
			ImGui::ColorPicker3("Selected Color", &selectedColor);
		if (textColorEditable)
			ImGui::ColorPicker3("Text Color", &textColor);

		ImGui::SliderFloat("Color Multiplier", &multiplier, 1, 5);
		ImGui::InputFloat("Fade Duration", &fadeDuration);
		ImGui::InputText("Text", text, IM_ARRAYSIZE(text));
		ImGui::DragFloat("Font Size", &checkboxText.Scale, 0.1, 0, 10);
		checkboxText.setOnlyText(text);

		ComponentOptions(this);
		ImGui::Separator();
	}
}

float2 CheckboxComponent::GetParentPosition()
{
	ComponentTransform2D* transform = owner->GetComponent<ComponentTransform2D>();
	return { transform->GetPosition().x - (strlen(text) * 12 * checkboxText.Scale) - (transform->GetScale().x / 4), transform->GetPosition().y - 5 };
}
bool CheckboxComponent::OnLoad(JsonParsing& node)
{
	checked = node.GetJsonBool("checked");
	return true;
}

bool CheckboxComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);

	/*file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "checked", checked);*/
	node.SetValueToArray(array, file.GetRootValue());

	return true;
}
