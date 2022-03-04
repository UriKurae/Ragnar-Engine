#include "Application.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"

#include "NavAgentComponent.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"

NavAgentComponent::NavAgentComponent()
{
	this->type = ComponentType::DETOUR;
}

NavAgentComponent::~NavAgentComponent()
{

}

bool NavAgentComponent::Update(float dt)
{

	return true;
}

void NavAgentComponent::OnEditor()
{

}

bool NavAgentComponent::OnLoad(JsonParsing& node)
{
	active = node.GetJsonBool("Active");

	return true;
}

bool NavAgentComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);

	node.SetValueToArray(array, file.GetRootValue());
	
	return true;
}
