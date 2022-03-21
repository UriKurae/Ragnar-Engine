#include "SliderComponent.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleInput.h"
#include "ModuleUI.h"
#include "ModuleEditor.h"
#include "ModuleCamera3D.h"

#include "GameObject.h"
#include "CameraComponent.h"
#include "MaterialComponent.h"
#include "Transform2DComponent.h"

#include "GameView.h"

SliderComponent::SliderComponent(GameObject* own)
{
	type = ComponentType::UI_SLIDER;
	value = 0;
	minValue = 70;
	maxValue = 120;
	drawRect = false;
	state = State::NORMAL;
	barProgres = 0.0f;
	completed = false;
	own->name = "Slider";
	actualColor = normalColor;

	sliderText.setText("Slider", 5, 5, 0.5, { 255,255,255 });
}

SliderComponent::~SliderComponent()
{
	RELEASE(planeToDraw);
	RELEASE(frontPlaneToDraw);
}

bool SliderComponent::Update(float dt)
{
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

	
	if (state == State::PRESSED) {
		float2 mousePos = { (float)app->input->GetMouseX() ,(float)app->input->GetMouseY() };
		float2 mPos = { ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y };
		float4 viewport = app->editor->GetGameView()->GetBounds();
		float2 fMousePos = { mPos.x - viewport.x , mPos.y - viewport.y };


		ComponentTransform2D* transform2D = owner->GetComponent<ComponentTransform2D>();
		float posXMin = ((viewport.z / 2) + (transform2D->GetPosition().x)) - (transform2D->GetButtonWidth() / 2);
		float posXMax = ((viewport.z / 2) + (transform2D->GetPosition().x)) + (transform2D->GetButtonWidth() / 2);

		if (fMousePos.x > posXMin && fMousePos.x < posXMax)
		{
			float thePos = fMousePos.x - posXMin;
			float total = posXMax - posXMin;


			barProgres = thePos / total;
			int cont = 0;
			ComponentTransform2D* q;
			for (int a = 0; a < owner->components.size(); a++) {
				if (owner->components[a]->type == ComponentType::TRANFORM2D)
				{
					
					cont++;
					if (cont == 1) {
						q = (ComponentTransform2D*)owner->components[a];
					}
					else
					{
						ComponentTransform2D* r = (ComponentTransform2D*)owner->components[a];
						float res = (viewport.z * 70) / 747;

						r->SetPosition(float3(thePos - (r->GetButtonWidth() * 5), q->GetPosition().y,r->GetPosition().z));
						r->SetButtonHeight(q->GetButtonHeight());
						r->Update(0);
						break;
					}
				}
			}
		}
	}
	return true;

}

void SliderComponent::Draw(CameraComponent* gameCam)
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
		actualColor = disabledColor;
		break;
	case State::PRESSED:
		glColor4f(pressedColor.r, pressedColor.g, pressedColor.b, pressedColor.a);
		actualColor = disabledColor;
		break;
	case State::SELECTED:
		glColor4f(selectedColor.r, selectedColor.g, selectedColor.b, selectedColor.a);
		actualColor = disabledColor;
		break;
	default:
		break;
	}
	firstDraw = false;
	MaterialComponent* mat = owner->GetComponent<MaterialComponent>();
	planeToDraw->DrawPlane2D(mat->GetTexture().get());
	firstDraw = true;

	frontPlaneToDraw->DrawPlane2D(secondMaterial->GetTexture().get());
	glDisable(GL_ALPHA_TEST);
	glColor3f(255, 255, 255);
}

void SliderComponent::OnEditor()
{
	// Colors
	static bool normalEditable = false;
	static bool pressedEditable = false;
	static bool focusedEditable = false;
	static bool disabledEditable = false;
	static bool selectedEditable = false;
	static bool textColorEditable = false;

	if (ImGui::CollapsingHeader("Slider"))
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

		ImGui::Text("Text Color"); ImGui::SameLine();
		if (ImGui::ColorButton("Text Color", ImVec4(textColor.r, textColor.g, textColor.b, textColor.a)))
			textColorEditable = !textColorEditable;

		sliderText.setOnlyColor({ textColor.r, textColor.g, textColor.b });

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

		ImGui::InputFloat("Min Value", &minValue);
		ImGui::InputFloat("Max Value", &maxValue);
		ImGui::SliderFloat("Value", &value, minValue, maxValue);

		ImGui::InputText("Text", text, IM_ARRAYSIZE(text));
		ImGui::DragFloat("Font Size", &sliderText.Scale, 0.1, 0, 10);
		sliderText.setOnlyText(text);

	}	
}

float2 SliderComponent::GetParentPosition()
{
	ComponentTransform2D* transform2D = owner->GetComponent<ComponentTransform2D>();
	float3 position = transform2D->GetPosition();
	return { position.x - (strlen(text) * 12 * sliderText.Scale), position.y - 5 };
}

bool SliderComponent::OnLoad(JsonParsing& node)
{
	planeToDraw = new MyPlane(float3{ 0,0,0 }, float3{ 1,1,1 });
	planeToDraw->own = owner;

	frontPlaneToDraw = new MyPlane(float3{ 0,0,0 }, float3{ 1,1,1 });
	frontPlaneToDraw->own = owner;
	int contt = 0;
	int contm = 0;
	for (int a = 0; a < owner->components.size(); a++) {

		if (owner->components[a]->type == ComponentType::TRANFORM2D)
		{
			contt++;
			if (contt==1) {
			
			}
			else
			{
				ComponentTransform2D* r = (ComponentTransform2D*)owner->components[a];
			}
				
		}
		else if (owner->components[a]->type == ComponentType::MATERIAL) 
		{
			contm++;
			if (contm == 1) {
			}
			else
			{
				secondMaterial = (MaterialComponent*)owner->components[a];			
			}
		}
	}
	owner->isUI = true;

	app->userInterface->UIGameObjects.push_back(owner);
	return true;
}

bool SliderComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "barProgres", barProgres);
	node.SetValueToArray(array, file.GetRootValue());

	return true;
}
