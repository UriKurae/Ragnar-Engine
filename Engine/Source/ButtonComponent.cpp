#include "ButtonComponent.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleInput.h"
#include "ModuleUI.h"

#include"GameObject.h"
#include "MaterialComponent.h"
#include "Transform2DComponent.h"
#include "GL/glew.h"

#include "Profiling.h"

ButtonComponent::ButtonComponent(GameObject* own)
{
	type = ComponentType::UI_BUTTON;
	own->isUI = true;
	active = true;
	buttonText.setText("Button", 5, 5, 0.5, { 255,255,255 });

	if (!own->GetComponent<ComponentTransform2D>()) // If comes from Load not enter
	{
		own->CreateComponent(ComponentType::TRANFORM2D);
		normalMaterial = (MaterialComponent*)own->CreateComponent(ComponentType::MATERIAL);
		focusedMaterial = (MaterialComponent*)own->CreateComponent(ComponentType::MATERIAL);
		pressedMaterial = (MaterialComponent*)own->CreateComponent(ComponentType::MATERIAL);
		disabledMaterial = (MaterialComponent*)own->CreateComponent(ComponentType::MATERIAL);
		actual = normalMaterial;
	}

	app->userInterface->UIGameObjects.push_back(own);
	planeToDraw = new MyPlane(float3{ 0,0,0 }, float3{ 1,1,1 });
	planeToDraw->own = own;
	app->userInterface->OrderButtons();
}

ButtonComponent::~ButtonComponent()
{
	RELEASE(planeToDraw);
}

bool ButtonComponent::Update(float dt)
{
	RG_PROFILING_FUNCTION("Button Update");
	buttonText.SetOnlyPosition(float2(GetParentPosition().x + textPos.x, GetParentPosition().y + textPos.y));

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
		}
		else
		{
			state = State::NORMAL;
			actual = normalMaterial;
		}
	}

	return true;
}

void ButtonComponent::Draw(CameraComponent* gameCam)
{
	if (owner->active) {
		glAlphaFunc(GL_GREATER, 0.5);
		glEnable(GL_ALPHA_TEST);

		planeToDraw->DrawPlane2D(actual->GetTexture().get());

		glDisable(GL_ALPHA_TEST);
		glColor3f(255, 255, 255);
	}
}

void ButtonComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("ButtonComponent"))
	{
		static float multiplier = 1;
		static float fadeDuration = 0.1f;

		// Manage if colors are being edited or not
		static bool textColorEditable = false;

		Checkbox(this, "Active", active);
		ImGui::Checkbox("Interactable", &active);

		ImGui::Separator();

		ImGui::DragFloat("Position X", &textPos.x, 0.2f);
		ImGui::DragFloat("Position Y", &textPos.y, 0.2f);

		ImGui::Text("Text Color"); ImGui::SameLine();
		if (ImGui::ColorButton("Text Color", ImVec4(textColor.r, textColor.g, textColor.b, textColor.a)))
			textColorEditable = !textColorEditable;

		if (textColorEditable)
			ImGui::ColorPicker3("Text Color", &textColor);

		buttonText.setOnlyColor({ textColor.r, textColor.g, textColor.b });

		ImGui::SliderFloat("Alpha", &alpha, 0.5f, 1.0f);

		ImGui::InputFloat("Fade Duration", &fadeDuration);


		if (ImGui::InputText("Text", text, IM_ARRAYSIZE(text)))
			buttonText.setOnlyText(text);
		ImGui::DragFloat("Font Size", &buttonText.Scale, 0.1, 0, 10);

		ComponentOptions(this);
		ImGui::Separator();
	}
}

float2 ButtonComponent::GetParentPosition()
{
	ComponentTransform2D* transform2D = owner->GetComponent<ComponentTransform2D>();
	float3 position = transform2D->GetPosition();

	return{ position.x / 2 ,position.y / 2 };
}
bool ButtonComponent::OnLoad(JsonParsing& node)
{
	std::string aux = node.GetJsonString("buttonText");
	strcpy(text, aux.c_str());
	buttonText.textt = text;

	fontScale = node.GetJsonNumber("fontScale");
	textColor.r = node.GetJsonNumber("textColor.r");
	textColor.g = node.GetJsonNumber("textColor.g");
	textColor.b = node.GetJsonNumber("textColor.b");

	textPos.x = node.GetJsonNumber("textPositionX");
	textPos.y = node.GetJsonNumber("textPositionY");
	alpha = node.GetJsonNumber("alpha");
	int cont = 0;

	for (int a = 0; a < owner->components.size(); a++) {
		if (owner->components[a]->type == ComponentType::MATERIAL)
		{
			switch (cont)
			{
			case 0:
				normalMaterial = (MaterialComponent*)owner->components[a];
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

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "buttonText", text);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "fontScale", fontScale);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.r", textColor.r);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.g", textColor.g);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.b", textColor.b);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textPositionX", textPos.x);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textPositionY", textPos.y);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "alpha", alpha);
	node.SetValueToArray(array, file.GetRootValue());

	return true;
}