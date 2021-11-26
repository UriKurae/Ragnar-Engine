#include "Application.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ModuleScene.h"
#include "Globals.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_internal.h"
#include "Imgui/ImGuizmo.h"

#include <stack>

#include "Profiling.h"

TransformComponent::TransformComponent(GameObject* own)
{
	type = ComponentType::TRANSFORM;
	owner = own;

	position = { 0.0f, 0.0f, 0.0f }; 
	rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	scale = { 1.0f, 1.0f, 1.0f };
	localMatrix = float4x4::FromTRS(position, rotation, scale);
	
	if (owner->GetParent() != nullptr)
	{
		TransformComponent* tr = owner->GetParent()->GetComponent<TransformComponent>();
		if (tr != nullptr)
			globalMatrix = localMatrix * tr->GetGlobalTransform();
	}
	else
	{
		globalMatrix = localMatrix;
	}

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
	if (changeTransform)
	{
		std::stack<GameObject*> stack;

		UpdateTransform();

		for (int i = 0; i < owner->GetChilds().size(); ++i)
			stack.push(owner->GetChilds()[i]);

		while (!stack.empty())
		{
			GameObject* go = stack.top();

			UpdateChildTransform(go);

			stack.pop();

			for (int i = 0; i < go->GetChilds().size(); ++i)
				stack.push(go->GetChilds()[i]);
		}

		SetAABB();

		changeTransform = false;
	}

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
	DEBUG_LOG("This is %s", owner->GetName());
}

void TransformComponent::SetTransform(float4x4 trMatrix)
{
	globalMatrix = trMatrix;
	globalMatrix.Decompose(position, rotation, scale);
	
	changeTransform = true;
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

bool TransformComponent::OnLoad(JsonParsing& node)
{
	active = node.GetJsonBool("Active");
	position = node.GetJson3Number(node, "Position");
	float4 quat = node.GetJson4Number(node, "Quaternion");
	rotation = Quat(quat.x, quat.y, quat.z, quat.w);
	scale = node.GetJson3Number(node, "Scale");
	rotationEditor = node.GetJson3Number(node, "RotationEditor");

	if (owner->GetParent() && owner->GetParent()->GetComponent<TransformComponent>() != nullptr)
	{
		SetParentTransform(owner->GetParent()->GetComponent<TransformComponent>());
	}
	else
	{
		SetTransform(position, rotation, scale);
	}

	RecursiveTransform(owner);

	return true;
}

bool TransformComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);
	file.SetNewJson3Number(file, "Position", position);
	file.SetNewJson4Number(file, "Quaternion", rotation);
	file.SetNewJson3Number(file, "RotationEditor", rotationEditor);
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

	if (owner->GetComponent<MeshComponent>())
	{
		//OBB newObb = owner->GetComponent<MeshComponent>()->GetLocalAABB();
		//newObb.Transform(globalMatrix);
		//owner->SetAABB(newObb);
		owner->SetAABB(owner->GetComponent<MeshComponent>()->GetLocalAABB());
	}
}

void TransformComponent::UpdateTransform()
{
	localMatrix = float4x4::FromTRS(position, rotation, scale);

	if (owner->GetParent() && owner->GetParent() != app->scene->GetRoot())
	{
		TransformComponent* parentTr = owner->GetParent()->GetComponent<TransformComponent>();
		if (parentTr) globalMatrix = parentTr->globalMatrix * localMatrix;
	}
	else
	{
		globalMatrix = localMatrix;
	}
}

void TransformComponent::UpdateChildTransform(GameObject* go)
{
	TransformComponent* transform = go->GetComponent<TransformComponent>();

	if (transform)
	{
		transform->globalMatrix = globalMatrix * transform->localMatrix;
	}
}

void TransformComponent::SetAABB()
{
	std::vector<GameObject*> goList = owner->GetChilds();
	owner->ClearAABB();
	for (int i = 0; i < goList.size(); ++i)
	{
		TransformComponent* tr = goList[i]->GetComponent<TransformComponent>();
		tr->SetAABB();
	}
	if (owner->GetComponent<MeshComponent>())
	{
		OBB newObb = owner->GetComponent<MeshComponent>()->GetLocalAABB().ToOBB();
		newObb.Transform(globalMatrix);
		owner->SetAABB(newObb);
	}
	app->scene->RemoveFromQuadtree(owner);
	app->scene->AddToQuadtree(owner);
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
	if (DrawVec3(std::string("Position: "), position)) changeTransform = true;

	if (DrawVec3(std::string("Rotation: "), rotationEditor))
	{
		if (rotationEditor.y == 0 && rotationEditor.z == 0) rotationOrder = RotationOrder::XYZ;
		else if (rotationEditor.x == 0 && rotationEditor.y == 0) rotationOrder = RotationOrder::ZYX;
		else if (rotationEditor.x == 0 && rotationEditor.z == 0) rotationOrder = RotationOrder::YXZ;

		Quat quaternionX = quaternionX.RotateX(math::DegToRad(rotationEditor.x));
		Quat quaternionY = quaternionY.RotateY(math::DegToRad(rotationEditor.y));
		Quat quaternionZ = quaternionZ.RotateZ(math::DegToRad(rotationEditor.z));

		switch (rotationOrder)
		{
		case RotationOrder::XYZ:
			rotation = quaternionX * quaternionY * quaternionZ;
			break;
		case RotationOrder::ZYX:
			rotation = quaternionZ * quaternionY * quaternionX;
			break;
		case RotationOrder::YXZ:
			rotation = quaternionY * quaternionX * quaternionZ;
			break;
		}

		changeTransform = true;
	}

	if (DrawVec3(std::string("Scale: "), scale)) changeTransform = true;
}