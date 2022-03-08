#include "Application.h"
#include "Transform2DComponent.h"
#include <math.h>
#include"CameraComponent.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"
#include "GameView.h"


ComponentTransform2D::ComponentTransform2D(float3 pos, float3 sca, float3 rot, GameObject* Own)
{
	//UID = GenerateUID();
	//type = ComponentType::TRANSFORM2D;
	own = Own;
	internalPosition = position = pos;
	scale.x = 1.30;
	scale.y = 0.5;
	scale.z = 1;
	rotationEuler = rot;
	generalScale = 1.0f;

	buttonWidth = 300;
	buttonHeight = 100;

	rotationQuat = FromEulerToQuat(rotationEuler);

	float4x4 aux;
	transMatrix = aux.FromTRS(internalPosition, rotationQuat, scale);
	transmat = transMatrix;
	transMatrix = transMatrix.Transposed();
	//matrix = transMatrix.ptr();
	
	//name = "Transform2D Component";

	//CreateAABB(ComponentType::PLANE, App->scene->gameObjects[App->scene->gameObjects.size() - 1], true);
}


ComponentTransform2D::~ComponentTransform2D()
{
}

void ComponentTransform2D::Enable()
{
}

bool ComponentTransform2D::Update(float dt)
{
	float4 viewport = app->editor->gameView->bounds;
	float temporalW = (viewport.z * 300) / 847;
	float temporalH = (viewport.w * 100) / 649;
	scale.x = (buttonWidth * 1.30) / temporalW;
	scale.y = (buttonHeight * 0.5) / temporalH;



	internalPosition.x = (position.x * 2.0f) / (viewport.z / 2);

	float res = (viewport.w * 1.5) / 649;
	res = 1.5 - (res - 1.5)+0.05;
	internalPosition.y = (position.y * res) / (viewport.w / 2);





	rotationQuat = FromEulerToQuat(rotationEuler);

	float4x4 aux;
	transMatrix = aux.FromTRS(internalPosition, rotationQuat, scale);
	transmat = transMatrix;
	transMatrix = transMatrix.Transposed();

	
	return true;
}
void ComponentTransform2D::Disable()
{
}

void ComponentTransform2D::OnEditor()
{
	if (ImGui::CollapsingHeader("2D Transform"))
	{
		ImGui::TextColored(ImVec4(0, 0, 255, 255), "Size");

		if (ImGui::DragFloat("Width", &buttonWidth, 0.5f, 0, 1000000))
		{
					
		}
		if (ImGui::DragFloat("Height", &buttonHeight, 0.5f, 0, 1000000))
		{
			
		}


		ImGui::TextColored(ImVec4(0, 0, 255, 255), "Position");
		static bool wasNull = true;


		if (ImGui::DragFloat("Position X", &position.x, 0.5f))
		{
				
		}
		if (ImGui::DragFloat("Position Y", &position.y, 0.5f))
		{
				
		}
	}
}

void ComponentTransform2D::setOwner() 
{
	matrix = transMatrix.ptr();
}



Quat ComponentTransform2D::FromEulerToQuat(float3 eulerAngles)
{
	eulerAngles.x = math::DegToRad(eulerAngles.x);
	eulerAngles.y = math::DegToRad(eulerAngles.y);
	eulerAngles.z = math::DegToRad(eulerAngles.z);

	Quat q = q.FromEulerXYZ(eulerAngles.x, eulerAngles.y, eulerAngles.z);

	return q;
}

float3 ComponentTransform2D::FromQuatToEuler(Quat quatAngles)
{
	float3 angles;

	angles = quatAngles.ToEulerXYZ();

	angles.x = math::RadToDeg(angles.x);
	angles.y = math::RadToDeg(angles.y);
	angles.z = math::RadToDeg(angles.z);
	float3 toReturn(angles.x,angles.y,angles.z);

	return toReturn;
}
bool ComponentTransform2D::OnLoad(JsonParsing& node)
{
	active = node.GetJsonBool("Active");
	position = node.GetJson3Number(node, "Position");
	scale = node.GetJson3Number(node, "Scale");
	rotationEuler = node.GetJson3Number(node, "rotationEuler");
	buttonWidth = node.GetJsonNumber("buttonWidth");
	buttonHeight = node.GetJsonNumber("buttonHeight");

	float4x4 aux;
	transMatrix = aux.FromTRS(position, rotationQuat, scale);
	transmat = transMatrix;
	transMatrix = transMatrix.Transposed();
	return true;
}

bool ComponentTransform2D::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();


	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJson3Number(file, "Position", position);
	file.SetNewJson3Number(file, "Scale", scale);
	file.SetNewJson3Number(file, "rotationEuler", rotationEuler);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "buttonWidth", buttonWidth);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "buttonHeight", buttonHeight);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}
