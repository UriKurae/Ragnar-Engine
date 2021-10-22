#include "TransformComponent.h"
#include "Globals.h"

#include "Imgui/imgui.h"

#include "mmgr/mmgr.h"

TransformComponent::TransformComponent()
{
	position = { 0.0f, 0.0f, 0.0f }; 
	rotation = { 0.0f, 1.0f, 1.0f, 1.0f };
	scale = { 1.0f, 1.0f, 1.0f };
	for (int i = 0; i < 3; ++i)
	{
		rotations[i] = 0.0f;
	}
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

Quat TransformComponent::AngleToQuat(float angle, int x, int y, int z)
{
	Quat quaternion;

	float rad = math::DegToRad(angle);

	float newQuaternionW = math::Cos(rad / 2);
	float newQuaternionX = x * math::Sin(rad / 2);
	float newQuaternionY = y * math::Sin(rad / 2);
	float newQuaternionZ = z * math::Sin(rad / 2);
	quaternion.x = newQuaternionX;
	quaternion.y = newQuaternionY;
	quaternion.z = newQuaternionZ;
	quaternion.w = newQuaternionW;

	quaternion.Normalize();

	return quaternion;
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
	if (ImGui::DragFloat3(" ", rotations))
	{	
		Quat quaternionX = AngleToQuat(rotations[0], 1, 0, 0);
		Quat quaternionY = AngleToQuat(rotations[1], 0, 1, 0);
		Quat quaternionZ = AngleToQuat(rotations[2], 0, 0, 1);

		Quat finalQuaternion = quaternionX * quaternionY * quaternionZ;
		
		SetTransform(position, finalQuaternion, scale);	
	}

	ImGui::Text("Scale: ");
	ImGui::SameLine(86.0f);
	if (ImGui::DragFloat3("-", scale.ptr()))
	{
		SetTransform(position, rotation, scale);
	}

}
