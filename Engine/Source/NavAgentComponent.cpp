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
	pathfinding->agents.push_back(this);
}

NavAgentComponent::~NavAgentComponent()
{
	delete agentProperties;
	agentProperties = nullptr;
}

bool NavAgentComponent::Update(float dt)
{
	//Scripting
	if (agentProperties->targetPosSet)
	{
		std::vector<float3> path;
		pathfinding->CalculatePath(this, agentProperties->targetPos, path);
		agentProperties->targetPosSet = false;
	}
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

	//Complete
	agentProperties->targetPos = node.GetJson3Number(node,"TargetPos");
	
	//agentProperties->targetPos = node.GetJsonNumber("TargetPos");
	agentProperties->targetPosSet = node.GetJsonNumber("TargetPosSet");

	agentProperties->pathType = (PathType)(int)node.GetJsonNumber("TargetPos");

	agentProperties->m_startRef = node.GetJsonNumber("TargetPos");
	agentProperties->m_endRef = node.GetJsonNumber("TargetPos");

	agentProperties->m_npolys = node.GetJsonNumber("TargetPos");
	//LoadArray(agentProperties->m_npolys,node, &(float)agentProperties->m_polys, "TargetPos");
	if (agentProperties->m_npolys > 0)
	{
		JSON_Array* array1 = node.GetJsonArray(node.ValueToObject(node.GetRootValue()),"TargetPos");
		JSON_Array* array2 = node.GetJsonArray(node.ValueToObject(node.GetRootValue()),"TargetPos");
		JSON_Array* array3 = node.GetJsonArray(node.ValueToObject(node.GetRootValue()),"TargetPos");
		JSON_Array* array4 = node.GetJsonArray(node.ValueToObject(node.GetRootValue()),"TargetPos");
		for (int i = 0; i < agentProperties->m_npolys; i++)
		{
			agentProperties->m_polys[i] = json_array_get_number(array1,i);
			agentProperties->m_straightPath[i] = json_array_get_number(array2, i);
			agentProperties->m_straightPathFlags[i] = json_array_get_number(array3, i);
			agentProperties->m_straightPathPolys[i] = json_array_get_number(array4, i);
		}

	}

	agentProperties->m_nsmoothPath = node.GetJsonNumber("TargetPos");
	if (agentProperties->m_nsmoothPath > 0)
	{
		JSON_Array* array = node.GetJsonArray(node.ValueToObject(node.GetRootValue()), "TargetPos");
		for (int i = 0; i < agentProperties->m_nsmoothPath; i++)
		{
			agentProperties->m_smoothPath[i] = json_array_get_number(array, i);
		}
	}

	agentProperties->m_nstraightPath = node.GetJsonNumber("TargetPos");


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
