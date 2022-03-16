#include "ImageComponent.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleInput.h"
#include"ModuleUI.h"

#include"GameObject.h"
#include "MaterialComponent.h"

ImageComponent::ImageComponent(GameObject* own)
{
	type = ComponentType::UI_IMAGE;
	this->text = "Image Component";
	own->name = "Image";

	planeToDraw = new MyPlane(float3{ 0,0,0 }, float3{ 1,1,1 });
	planeToDraw->own = owner;
}

ImageComponent::~ImageComponent()
{
	text.clear();
	RELEASE(planeToDraw);
}

bool ImageComponent::Update(float dt)
{
	return true;
}

void ImageComponent::Draw(CameraComponent* gameCam)
{
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
}
bool ImageComponent::OnLoad(JsonParsing& node)
{
	/*if(planeToDraw!=nullptr)
		delete planeToDraw;*/
	RELEASE(planeToDraw);
	planeToDraw = new MyPlane(float3{ 0,0,0 }, float3{ 1,1,1 });
	planeToDraw->own = owner;
	owner->isUI = true;

	app->userInterface->UIGameObjects.push_back(owner);
	/*text = node.GetJsonNumber("buttonText");
	buttonText.textt = text;
	fontScale = node.GetJsonNumber("fontScale");
	textColor.r = node.GetJsonNumber("textColor.r");
	textColor.g = node.GetJsonNumber("textColor.g");
	textColor.b = node.GetJsonNumber("textColor.b");*/
	return true;
}

bool ImageComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	node.SetValueToArray(array, file.GetRootValue());
	/*file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "buttonText", text.c_str());
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "fontScale", fontScale);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.r", textColor.r);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.g", textColor.g);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "textColor.b", textColor.b);
	node.SetValueToArray(array, file.GetRootValue());*/

	return true;
}
