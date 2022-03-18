#include "ButtonComponent.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleInput.h"
#include "ModuleUI.h"

#include"GameObject.h"
#include "MaterialComponent.h"

ButtonComponent::ButtonComponent(GameObject* own)
{
	active = true;
	own->name = "Button";
	type = ComponentType::UI_BUTTON;
	buttonText.setText("Button", 5, 5, 0.5, { 255,255,255 });
	
}

ButtonComponent::~ButtonComponent()
{
	RELEASE(planeToDraw);
}

bool ButtonComponent::Update(float dt)
{
	buttonText.SetOnlyPosition(float2(GetParentPosition().x/2, GetParentPosition().y/2));

	if (!active)
		state = State::DISABLED;
	else
		state = State::NORMAL;

	if (state != State::DISABLED)
	{		
		if (app->userInterface->focusedGameObject == owner)
		{
			state = State::FOCUSED;
			actual = focusedMaterial;
			if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) 
			{
				state = State::PRESSED;
				actual = pressedMaterial;
			}
				

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				
			}
		}
		else 
		{
			state = State::NORMAL;
			actual = normalMaterial;
		}

		/*if (app->userInterface->UIGameObjectSelected == owner)
		{
			state = State::SELECTED;
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) 
			{

			}				
		}*/
	}

	return true;
}

void ButtonComponent::Draw(CameraComponent* gameCam)
{
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	planeToDraw->DrawPlane2D(actual->GetTexture().get());
	
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


		ImGui::Separator();

		ImGui::Text("Text Color"); ImGui::SameLine();
		if (ImGui::ColorButton("Text Color", ImVec4(textColor.r, textColor.g, textColor.b, textColor.a)))
			textColorEditable = !textColorEditable;

		if (textColorEditable)
			ImGui::ColorPicker3("Text Color", &textColor);

		buttonText.setOnlyColor({ textColor.r, textColor.g, textColor.b });


		ImGui::SliderFloat("Color Multiplier", &multiplier, 1, 5);
		ImGui::InputFloat("Fade Duration", &fadeDuration);

		ImGui::InputText("Text", text, IM_ARRAYSIZE(text));
		ImGui::DragFloat("Font Size", &buttonText.Scale, 0.1, 0, 10);
		buttonText.setOnlyText(text);
		
		ComponentOptions(this);
		ImGui::Separator();
	}
}

float2 ButtonComponent::GetParentPosition()
{
	ComponentTransform2D* transform2D = owner->GetComponent<ComponentTransform2D>();
	float3 position = transform2D->GetPosition();
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

	int cont = 0;

	for (int a = 0; a < owner->components.size(); a++) {
		if (owner->components[a]->type == ComponentType::MATERIAL)
		{
			switch (cont)
			{
			case 0:
				normalMaterial=(MaterialComponent*)owner->components[a];
				break;
			case 1:
				focusedMaterial = (MaterialComponent*)owner->components[a];
				break;
			case 2:
				pressedMaterial = (MaterialComponent*)owner->components[a];
				break;
			case 3:
				disabledMaterial = (MaterialComponent*)owner->components[a];
				actual = normalMaterial;
				break;
			default:
				break;
			}
			cont++;
		}
	}
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