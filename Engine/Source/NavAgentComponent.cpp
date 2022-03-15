#include "Application.h"
#include "Globals.h"
#include "NavAgentComponent.h"

#include "ModuleNavMesh.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"

NavAgentComponent::NavAgentComponent(GameObject* obj) : Component()
{
	owner = obj;
	this->type = ComponentType::NAVAGENT;
	agentProperties = new NavAgent;
	pathfinding = app->navMesh->GetPathfinding();
}

NavAgentComponent::~NavAgentComponent()
{
	delete agentProperties;
	agentProperties = nullptr;
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
		ImGui::Spacing();
		ImGui::Text("Agent Properties");
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::PushItemWidth(180);
		ImGui::DragFloat("Agent Radius", &agentProperties->radius, 0.1f);
		ImGui::DragFloat("Agent Height", &agentProperties->height, 0.1f);
		ImGui::DragFloat("Stop Height", &agentProperties->maxClimb, 0.1f);
		ImGui::DragInt("Max Slope", &agentProperties->maxSlope, 1);
		ImGui::PopItemWidth();
		ImGui::Dummy({ 0,10 });

		ImGui::Text("Steering");
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::PushItemWidth(180);
		ImGui::DragFloat("Speed", &agentProperties->speed, 0.1f);
		ImGui::DragFloat("Angular Speed", &agentProperties->angularSpeed, 0.1f);
		ImGui::DragFloat("Acceleration", &agentProperties->acceleration, 0.1f);
		ImGui::DragFloat("Stopping Distance", &agentProperties->stoppingDistance, 0.1f);
		ImGui::PopItemWidth();
		ImGui::Dummy({ 0,10 });

		ImGui::Text("Path Type");
		ImGui::Separator();
		ImGui::Spacing();
		if (ImGui::RadioButton("Smooth Path", agentProperties->pathType == PathType::SMOOTH))
			agentProperties->pathType = PathType::SMOOTH;
		ImGui::SameLine();
		if (ImGui::RadioButton("Straight Path", agentProperties->pathType == PathType::STRAIGHT))
			agentProperties->pathType = PathType::STRAIGHT;

		ImGui::Spacing();
		ComponentOptions(this);
	}
	ImGui::PopID();
}

bool NavAgentComponent::OnLoad(JsonParsing& node)
{
	active = node.GetJsonBool("Active");

	agentProperties->radius = node.GetJsonNumber("Radius");
	agentProperties->height = node.GetJsonNumber("Height");
	agentProperties->maxClimb = node.GetJsonNumber("StopHeight");
	agentProperties->maxSlope = node.GetJsonNumber("MaxSlope");

	agentProperties->speed = node.GetJsonNumber("Speed");
	agentProperties->angularSpeed = node.GetJsonNumber("AngularSpeed");
	agentProperties->acceleration = node.GetJsonNumber("Acceleration");
	agentProperties->stoppingDistance = node.GetJsonNumber("StoppingDistance");

	return true;
}

bool NavAgentComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Radius", (float)agentProperties->radius);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Height", (float)agentProperties->height);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "StopHeight", (float)agentProperties->maxClimb);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "MaxSlope", (int)agentProperties->maxSlope);

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Speed", (float)agentProperties->speed);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "AngularSpeed", (float)agentProperties->angularSpeed);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Acceleration", (float)agentProperties->acceleration);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "StoppingDistance", (float)agentProperties->stoppingDistance);

	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);

	node.SetValueToArray(array, file.GetRootValue());
	
	return true;
}
