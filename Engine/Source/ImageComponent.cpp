#include "Application.h"
#include"GameObject.h"
#include "ModuleInput.h"
#include"ModuleUI.h"
#include "ImageComponent.h"
#include "MaterialComponent.h"

ImageComponent::ImageComponent(GameObject* own)
{
	//name = "Image Component";
	type = ComponentType::UI_IMAGE;
	this->text = "Image Component";
	owner = own;
	state = State::NORMAL;
	actualColor = color;
	/*color = White;
	UIid = id;*/
}

ImageComponent::~ImageComponent()
{
	text.clear();
}

bool ImageComponent::Update(float dt)
{
	return true;
}

void ImageComponent::Draw(CameraComponent* gameCam)
{
	/*MyPlane* planeToDraw = nullptr;
	int auxId = owner->id;

	for (int i = 0; i < App->editor->planes.size(); i++)
		if (App->editor->planes[i]->id == auxId) planeToDraw = App->editor->planes[i];*/

	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	glColor4f(color.r, color.g, color.b, color.a);

	MaterialComponent* mat = owner->GetComponent<MaterialComponent>();
	planeToDraw->DrawPlane2D(mat->GetTexture().get());

	glDisable(GL_ALPHA_TEST);
	glColor3f(255, 255, 255);
}

void ImageComponent::OnEditor()
{
	// Manage if colors are being edited or not
	/*static bool normalEditable = false;

	ImGui::Checkbox("Active", &active);

	ImGui::Text("Image Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Image Color", ImVec4(color.r, color.g, color.b, color.a)))
		normalEditable = !normalEditable;

	if (normalEditable)
	{
		ImGui::ColorPicker3("Image Color", &color);
	}*/
}
bool ImageComponent::OnLoad(JsonParsing& node)
{
	planeToDraw = new MyPlane(float3{ 0,0,0 }, float3{ 1,1,1 });
	planeToDraw->own = owner;
	owner->isUI = true;

	app->userInterface->UIGameObjects.push_back(owner);
	return true;
}

bool ImageComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}
