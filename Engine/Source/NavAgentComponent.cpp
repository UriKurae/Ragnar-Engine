#include "Application.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"

#include "NavAgentComponent.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"

NavAgentComponent::NavAgentComponent(GameObject* own, TransformComponent* trans)
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
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader(ICON_FA_CUBES" Recast"))
	{
		ImGui::Separator();
		ImGui::Columns(2, NULL, false);
		ImGui::Spacing();
		ImGui::Text("Steering");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Speed");
		ImGui::Spacing();
		ImGui::Text("Angular Speed");
		ImGui::Spacing();
		ImGui::Text("Stopping Distance");
		ImGui::Spacing();
		ImGui::NextColumn();

		char buffer[50];

		ImGui::Dummy({ 0,17 });
		sprintf_s(buffer, 50, "%.2f", properties.speed);
		if (ImGui::InputText("##speed", &buffer[0], sizeof(buffer)))
		{
			if (buffer[0] != '\0') {
				properties.speed = strtod(buffer, NULL);
			}
		}

		sprintf_s(buffer, 50, "%.2f", properties.angularSpeed);
		if (ImGui::InputText("##angularSpeed", &buffer[0], sizeof(buffer)))
		{
			if (buffer[0] != '\0') {
				properties.angularSpeed = strtod(buffer, NULL);
			}
		}

		sprintf_s(buffer, 50, "%.2f", properties.stoppingDistance);
		if (ImGui::InputText("##stoppingDistance", &buffer[0], sizeof(buffer)))
		{
			if (buffer[0] != '\0') {
				properties.stoppingDistance = strtod(buffer, NULL);
			}
		}


		ImGui::NextColumn();
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
