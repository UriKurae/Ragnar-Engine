#include "Application.h"
#include "SDL.h"
#include "CheckboxComponent.h"
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
	actualColor = normalColor;
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
				if (actual == noSelectedMaterial) {
					actual = selectedMaterial;
					checked = true;
				}
				else 
				{
					checked = false;
					actual = noSelectedMaterial;
				}
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

	//MaterialComponent* mat = owner->GetComponent<MaterialComponent>();


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

		/*ImGui::Text("Text Color"); ImGui::SameLine();
		if (ImGui::ColorButton("Text Color", ImVec4(textColor.r, textColor.g, textColor.b, textColor.a)))
			textColorEditable = !textColorEditable;

		checkboxText.setOnlyColor({ textColor.r, textColor.g, textColor.b });*/

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
}


float2 CheckboxComponent::GetParentPosition()
{
	ComponentTransform2D* transform = owner->GetComponent<ComponentTransform2D>();
	return { transform->position.x - (strlen(text) * 12 * checkboxText.Scale) - (transform->scale.x / 4), transform->position.y - 5 };
}
bool CheckboxComponent::OnLoad(JsonParsing& node)
{
	std::string selected;
	selected = node.GetJsonString("selected");
	noSelectedMaterial = nullptr;
	selectedMaterial= nullptr;
	for (int a = 0; a < owner->components.size(); a++) {
		if (owner->components[a]->type == ComponentType::MATERIAL) 
		{
			if (selectedMaterial == nullptr) {
				selectedMaterial = (MaterialComponent*)owner->components[a];
			}
			else 
			{
				noSelectedMaterial = (MaterialComponent*)owner->components[a];
				break;
			}
		}
	}
	
	const char* sel=new char[selected.size()];
	sel = selected.c_str();
	if (sel[0] == 'n')
	{
		actual = noSelectedMaterial;			
	}
	else 
	{
		actual = selectedMaterial;	
	}
	planeToDraw = new MyPlane(float3{ 0,0,0 }, float3{ 1,1,1 });
	planeToDraw->own = owner;
	owner->isUI = true;
	app->userInterface->UIGameObjects.push_back(owner);

	/*delete[] sel;*/
	

	return true;
}

bool CheckboxComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();


	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	if (actual == noSelectedMaterial) {
		file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "selected", "noSelected");
	}
	else {
		file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "selected", "Selected");
	}


	node.SetValueToArray(array, file.GetRootValue());

	return true;
}
