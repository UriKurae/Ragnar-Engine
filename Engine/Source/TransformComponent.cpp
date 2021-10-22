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

Quat TransformComponent::AngleToQuat(float angle)
{
	Quat quaternion;

	float rad = math::DegToRad(angle);

	float newQuaternionW = math::Cos(rad / 2);
	float newQuaternionX = rotations[0] * math::Sin(rad / 2);
	float newQuaternionY = rotations[1] * math::Sin(rad / 2);
	float newQuaternionZ = rotations[2] * math::Sin(rad / 2);
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
	float aux[3] = {rotations[0], rotations[1], rotations[2]};
	if (ImGui::DragFloat3(" ", rotations))
	{
		Quat finalQuaternion = {};
			Quat quaternionX = AngleToQuat(rotations[0]);
			Quat quaternionY = AngleToQuat(rotations[1]);
			Quat quaternionZ = AngleToQuat(rotations[2]);
			if (rotations[0] != aux[0])
				finalQuaternion = quaternionX;
			else if (rotations[1] != aux[1])
				finalQuaternion = quaternionY;
			else if (rotations[2] != aux[2])
				finalQuaternion = quaternionZ;

				
		DEBUG_LOG("QUATX IS %f %f %f %f ", finalQuaternion.x, finalQuaternion.y, finalQuaternion.z, finalQuaternion.w);
		
		SetTransform(position, finalQuaternion, scale);	
	}

	ImGui::Text("Scale: ");
	ImGui::SameLine(86.0f);
	if (ImGui::DragFloat3("-", scale.ptr()))
	{
		SetTransform(position, rotation, scale);
	}

}
