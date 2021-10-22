#include "TransformComponent.h"
#include "Globals.h"

#include "Imgui/imgui.h"

#include "mmgr/mmgr.h"

TransformComponent::TransformComponent()
{
	position = { 0.0f, 0.0f, 0.0f }; 
	rotation = { 0.0f, 1.0f, 1.0f, 1.0f };
	scale = { 1.0f, 1.0f, 1.0f };
}

TransformComponent::~TransformComponent()
{
}

bool TransformComponent::Update(float dt)
{
	return true;
}

void TransformComponent::OnEditor()
{
	ImGui::SetNextTreeNodeOpen(true);
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::PushItemWidth(90);
		//std::string test = std::to_string(position.x);
		//char* pos = new char[test.length()];
		//strcpy(pos, test.c_str());
		
		ShowTransformationInfo();

		ImGui::Separator();
	}
}

void TransformComponent::SetTransform(float3 pos, Quat rot, float3 sca)
{
	position = pos;
	rotation = rot;
	scale = sca;

	transform = float4x4::FromTRS(position, rotation, scale);
}

void TransformComponent::ShowTransformationInfo()
{
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);

	ImGui::Text("Position: ");
	ImGui::SameLine();
	if (ImGui::DragFloat3(".", position.ptr()))
	{
		SetTransform(position, rotation, scale);
	}

	ImGui::Text("Rotation: ");
	ImGui::SameLine();
	if (ImGui::DragFloat3(" ", rotation.ptr()))
	{
		SetTransform(position, rotation, scale);
	}

	ImGui::Text("Scale: ");
	ImGui::SameLine(86.0f);
	if (ImGui::DragFloat3("-", scale.ptr()))
	{
		SetTransform(position, rotation, scale);
	}

}
