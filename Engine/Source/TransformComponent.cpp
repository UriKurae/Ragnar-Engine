#include "GameObject.h"
#include "TransformComponent.h"
#include "Globals.h"

#include "Imgui/imgui.h"

#include "mmgr/mmgr.h"

TransformComponent::TransformComponent(GameObject* own)
{
	position = { 0.0f, 0.0f, 0.0f }; 
	rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	scale = { 1.0f, 1.0f, 1.0f };
	globalMatrix = float4x4::FromTRS(position, rotation, scale);

	owner = own;

	for (int i = 0; i < 3; ++i)
		rotationEditor[i] = 0;

	changeTransform = false;
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


	globalMatrix = float4x4::FromTRS(position, rotation, scale);
}

void TransformComponent::SetTranslation(float3 pos)
{
	SetTransform(pos, rotation, scale);
}

void TransformComponent::SetRotation(Quat rot)
{
	SetTransform(position, rot, scale);
}

void TransformComponent::SetScale(float3 sca)
{
	SetTransform(position, rotation, sca);
}

void TransformComponent::SetChildTransform(float3 pos, Quat rot, float3 sca)
{
	float3 scaleParent(scale.x * sca.x, scale.y * sca.y, scale.z * sca.z);
	globalMatrix = float4x4::FromTRS(position + pos, rotation * rot, scaleParent);
}

void TransformComponent::SetParentTransform(TransformComponent* component)
{
	float3 pos = position + component->GetPosition();
	float3 scaleParent;
	scaleParent.x = scale.x * component->GetScale().x;
	scaleParent.y = scale.y * component->GetScale().y;
	scaleParent.z = scale.z * component->GetScale().z;
	Quat rot = rotation * component->GetRotation();
	globalMatrix = float4x4::FromTRS(pos, rot, scaleParent);
}

void TransformComponent::RecursiveTransform(GameObject* parent)
{
	std::vector<GameObject*> children = parent->GetChilds();
	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->GetComponent<TransformComponent>()->SetChildTransform(position, rotation, scale);
		RecursiveTransform(children[i]);
	}
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
		if (owner->GetParent() != nullptr && owner->GetParent()->GetComponent<TransformComponent>() != nullptr)
		{
			SetParentTransform(owner->GetParent()->GetComponent<TransformComponent>());
		}
		else
			SetTranslation(position);

		RecursiveTransform(owner);
	}

	ImGui::Text("Rotation: ");
	ImGui::SameLine();
	if (ImGui::DragFloat3(" ", rotationEditor))
	{	
		Quat quaternionX = quaternionX.RotateX(math::DegToRad(rotationEditor[0]));
		Quat quaternionY = quaternionY.RotateY(math::DegToRad(rotationEditor[1]));
		Quat quaternionZ = quaternionZ.RotateZ(math::DegToRad(rotationEditor[2]));
		Quat finalQuaternion = quaternionX * quaternionY * quaternionZ;
		rotation = finalQuaternion;
		if (owner->GetParent() != nullptr && owner->GetParent()->GetComponent<TransformComponent>() != nullptr)
		{
			SetParentTransform(owner->GetParent()->GetComponent<TransformComponent>());
		}
		else
			SetRotation(finalQuaternion);

		RecursiveTransform(owner);
	}

	ImGui::Text("Scale: ");
	ImGui::SameLine(86.0f);
	if (ImGui::DragFloat3("-", scale.ptr()))
	{
		if (owner->GetParent() != nullptr && owner->GetParent()->GetComponent<TransformComponent>() != nullptr)
		{
			SetParentTransform(owner->GetParent()->GetComponent<TransformComponent>());
		}
		else
			SetScale(scale);

		RecursiveTransform(owner);
	}
}