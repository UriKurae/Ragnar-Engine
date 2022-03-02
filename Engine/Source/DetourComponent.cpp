#include "Application.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"

#include "DetourComponent.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"

DetourComponent::DetourComponent()
{
	this->type = ComponentType::DETOUR;
}

DetourComponent::~DetourComponent()
{

}

bool DetourComponent::Update(float dt)
{

	return true;
}

void DetourComponent::OnEditor()
{

}

bool DetourComponent::OnLoad(JsonParsing& node)
{
	active = node.GetJsonBool("Active");

	return true;
}

bool DetourComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);

	node.SetValueToArray(array, file.GetRootValue());
	
	return true;
}
