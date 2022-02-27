#include "Application.h"
#include "SDL.h"
#include "CheckboxComponent.h"
#include"glew/include/GL/glew.h"
#include"ModuleUI.h"
#include"ModuleInput.h"
CheckboxComponent::CheckboxComponent(GameObject* ow)
{
	//name = "CheckBox Component";
	type = ComponentType::UI_CHECKBOX;
	state = State::NORMAL;
	checked = false;
	checkboxText.setText("check", 5, 5, 0.5, { 255,255,255 });
	//UIid = id;
	gen= ow;
	firstTime = true;
}

CheckboxComponent::~CheckboxComponent()
{

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

			if (state != State::FOCUSED && state != State::PRESSED)
			{
				/*app->audio->PlayFx(focusedFX);*/
			}

			if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = State::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				if (firstTime)
				{
					OnClick();
					OnClick();
					OnClick();
					firstTime = false;
				}
				else
				{
					OnClick();
				}
			}
		}
		else state = State::NORMAL;

		if (app->userInterface->UIGameObjectSelected == owner)
		{
			state = State::SELECTED;
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
			{
				if (firstTime)
				{
					OnClick();
					OnClick();
					OnClick();
					firstTime = false;
				}
				else
				{
					OnClick();
				}
			}
		}
	}
	return true;
}

void CheckboxComponent::Draw()
{
	

	

	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	switch (state)
	{
	case State::DISABLED:
		glColor4f(disabledColor.r, disabledColor.g, disabledColor.b, disabledColor.a);
		break;
	case State::NORMAL:
		glColor4f(normalColor.r, normalColor.g, normalColor.b, normalColor.a);
		break;
	case State::FOCUSED:
		glColor4f(focusedColor.r, focusedColor.g, focusedColor.b, focusedColor.a);
		break;
	case State::PRESSED:
		glColor4f(pressedColor.r, pressedColor.g, pressedColor.b, pressedColor.a);
		break;
	case State::SELECTED:
		glColor4f(selectedColor.r, selectedColor.g, selectedColor.b, selectedColor.a);
		break;
	default:
		break;
	}

	planeToDraw->DrawPlane2D();

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

	checkboxText.setOnlyColor({ textColor.r, textColor.g, textColor.b });

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
	ImGui::DragFloat("Font Size", &checkboxText.Scale, 0.1, 0, 10);
	checkboxText.setOnlyText(text);
}

void CheckboxComponent::OnClick()
{
	/*checked = !checked;

	if (owner != nullptr)
	{
		App->editor->objectSelected = owner;

		int oldMaterialId;
		oldMaterialId = owner->SearchComponent(owner, ComponentType::MATERIAL);
		if (oldMaterialId != -1)
		{
			owner->components.erase(owner->components.begin() + oldMaterialId);
		}

		else
		{
			if (checked)
			{
				owner->CreateComponent(ComponentType::MATERIAL, "Library/Textures/TextCheckboxTrue.dds", true);
				if (UIid == 3) App->maxMs = 1000 / 60;
				else
				{
					for (int i = 0; i < App->userInterface->UIGameObjects.size(); i++)
					{
						GameObject* auxGo = App->userInterface->UIGameObjects[i];
						uint comp = auxGo->SearchComponent(auxGo, ComponentType::UI_IMAGE);
						if (comp != -1 && auxGo->components[comp]->UIid == 10) auxGo->getTransform2D()->position.x = 0;
					}
				}
			}
			else
			{
				owner->CreateComponent(ComponentType::MATERIAL, "Library/Textures/TextCheckboxFalse.dds", true);
				if (UIid == 3) App->maxMs = 0;
				else
				{
					for (int i = 0; i < App->userInterface->UIGameObjects.size(); i++)
					{
						GameObject* auxGo = App->userInterface->UIGameObjects[i];
						uint comp = auxGo->SearchComponent(auxGo, ComponentType::UI_IMAGE);
						if (comp != -1 && auxGo->components[comp]->UIid == 10) auxGo->getTransform2D()->position.x = 70000;
					}
				}
			}

			owner->components[owner->components.size() - 1]->owner = owner;
		}
	}*/
}

float2 CheckboxComponent::GetParentPosition()
{
	ComponentTransform2D* transform = gen->GetComponent<ComponentTransform2D>();
	return { transform->position.x - (strlen(text) * 12 * checkboxText.Scale) - (transform->scale.x / 4), transform->position.y - 5 };
}

