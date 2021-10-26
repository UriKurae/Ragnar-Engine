#include "GameObject.h"
#include "TransformComponent.h"
#include "Globals.h"

#include "Imgui/imgui.h"

#include "mmgr/mmgr.h"

TransformComponent::TransformComponent()
{
	position = { 0.0f, 0.0f, 0.0f }; 
	rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	scale = { 1.0f, 1.0f, 1.0f };
	transform = float4x4::FromTRS(position, rotation, scale);
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

void TransformComponent::SetTranslation(float3 pos)
{
	position = pos;
	SetTransform(position, rotation, scale);
}

void TransformComponent::SetRotation(Quat rot)
{
	rotation = rot;
	SetTransform(position, rotation, scale);
}

void TransformComponent::SetScale(float3 sca)
{
	scale = sca;
	SetTransform(position, rotation, scale);
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
		SetTranslation(position);
		std::vector<GameObject*> children = owner->GetChilds();
		for (int i = 0; i < children.size(); ++i)
		{
			children[i]->GetComponent<TransformComponent>()->SetTranslation(position);
		}
	}

	static float rotations[3] = { rotation.x,rotation.y, rotation.z };
	ImGui::Text("Rotation: ");
	ImGui::SameLine();
	if (ImGui::DragFloat3(" ", rotations))
	{	
		Quat quaternionX = quaternionX.RotateX(math::DegToRad(rotations[0]));
		Quat quaternionY = quaternionY.RotateY(math::DegToRad(rotations[1]));
		Quat quaternionZ = quaternionZ.RotateZ(math::DegToRad(rotations[2]));
		Quat finalQuaternion = quaternionX * quaternionY * quaternionZ;
		
		SetRotation(finalQuaternion);

		std::vector<GameObject*> children = owner->GetChilds();
		for (int i = 0; i < children.size(); ++i)
		{
			children[i]->GetComponent<TransformComponent>()->SetRotation(finalQuaternion);
		}
	}

	ImGui::Text("Scale: ");
	ImGui::SameLine(86.0f);
	if (ImGui::DragFloat3("-", scale.ptr()))
	{
		SetScale(scale);
		std::vector<GameObject*> children = owner->GetChilds();
		for (int i = 0; i < children.size(); ++i)
		{
			children[i]->GetComponent<TransformComponent>()->SetScale(scale);
		}
	}

}
