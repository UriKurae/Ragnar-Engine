#include "Application.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"

#include "NavAgentComponent.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"

NavAgentComponent::NavAgentComponent(GameObject* own, TransformComponent* trans)
{
	this->type = ComponentType::NAVAGENT;
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
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader(ICON_FA_RUNNING" NavAgent"))
	{
		ImGui::Text("Steering");
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::PushItemWidth(180);
		ImGui::DragFloat("Speed", &properties.speed, 0.1f);
		ImGui::DragFloat("Angular", &properties.angularSpeed, 0.1f);
		ImGui::DragFloat("Stopping Distance", &properties.stoppingDistance, 0.1f);
		ImGui::PopItemWidth();

		ComponentOptions(this);
	}
	ImGui::PopID();
}

bool NavAgentComponent::OnLoad(JsonParsing& node)
{
	active = node.GetJsonBool("Active");
	properties.speed = node.GetJsonNumber("Speed");
	properties.angularSpeed = node.GetJsonNumber("Angular Speed");
	properties.stoppingDistance = node.GetJsonNumber("Stopping Distance");

	return true;
}

bool NavAgentComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Speed", (float)properties.speed);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Angular Speed", (float)properties.angularSpeed);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Stopping Distance", (float)properties.stoppingDistance);

	node.SetValueToArray(array, file.GetRootValue());
	
	return true;
}
