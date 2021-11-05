#include "GameObject.h"
#include "TransformComponent.h"
#include "Globals.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_internal.h"

#include "Profiling.h"

TransformComponent::TransformComponent(GameObject* own)
{
	type = ComponentType::TRANSFORM;
	position = { 0.0f, 0.0f, 0.0f }; 
	rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	scale = { 1.0f, 1.0f, 1.0f };
	globalMatrix = float4x4::FromTRS(position, rotation, scale);

	owner = own;

	for (int i = 0; i < 3; ++i)
		rotationEditor[i] = 0;

	collapsed = false;
	active = true;
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

bool TransformComponent::OnLoad(JsonParsing& node, JSON_Array* array)
{
	return true;
}

bool TransformComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJson3Number(file, "Position", position);
	file.SetNewJson4Number(file, "Quaternion", rotation);
	file.SetNewJson3Number(file, "Scale", scale);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
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

bool TransformComponent::DrawVec3(std::string& name, float3& vec)
{
	float3 lastVec = vec;
	ImGui::PushID(name.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 100.0f);
	ImGui::Text(name.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	ImGui::Button("X");
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &vec.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
	ImGui::Button("Y");
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &vec.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
	ImGui::Button("Z");
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &vec.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();

	if (lastVec.x != vec.x || lastVec.y != vec.y || lastVec.z != vec.z) return true;
	else return false;
}

void TransformComponent::ShowTransformationInfo()
{
	float3 pos = position;
	float3 rot;
	float3 sca;
	/*ImGui::Text("Position: ");
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
	}*/

	
	if (DrawVec3(std::string("Position: "), position))
	{
		if (owner->GetParent() != nullptr && owner->GetParent()->GetComponent<TransformComponent>() != nullptr)
		{
			SetParentTransform(owner->GetParent()->GetComponent<TransformComponent>());
		}
		else
			SetTranslation(position);

		RecursiveTransform(owner);
	}
	//{
	//	if (owner->GetParent() != nullptr && owner->GetParent()->GetComponent<TransformComponent>() != nullptr)
	//	{
	//		SetParentTransform(owner->GetParent()->GetComponent<TransformComponent>());
	//	}
	//	else
	//		SetTranslation(position);

	//	RecursiveTransform(owner);
	//}

	if (DrawVec3(std::string("Rotation: "), rotationEditor))
	{	
		Quat quaternionX = quaternionX.RotateX(math::DegToRad(rotationEditor.x));
		Quat quaternionY = quaternionY.RotateY(math::DegToRad(rotationEditor.y));
		Quat quaternionZ = quaternionZ.RotateZ(math::DegToRad(rotationEditor.z));
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

	if (DrawVec3(std::string("Scale: "), scale))
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