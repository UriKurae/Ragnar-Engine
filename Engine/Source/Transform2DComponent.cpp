#include "Transform2DComponent.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleSceneManager.h"
#include "Scene.h"

#include "ModuleEditor.h"
#include "ModuleUI.h"

#include "GameObject.h"
#include "GameView.h"

#include <math.h>


ComponentTransform2D::ComponentTransform2D(/*float3 pos, float3 sca, float3 rot,*/ GameObject* own)
{
	internalPosition = { 0,0,0 };
	position = { 0,0,84.5f };
	scale.x = 30;
	scale.y = 15;
	scale.z = 1;

	//rotationEuler = rot;
	rotationEuler = float3(0, 0, 0);
	buttonWidth = 300;
	buttonHeight = 100;

	rotationQuat = FromEulerToQuat(rotationEuler);
	type = ComponentType::TRANFORM2D;
	float4x4 aux;
	transMatrix = aux.FromTRS(internalPosition, rotationQuat, scale);
	transmat = transMatrix;
	transMatrix = transMatrix.Transposed();
	type = ComponentType::TRANFORM2D;	
}

ComponentTransform2D::~ComponentTransform2D()
{
	app->userInterface->DeleteUIGameObjects(owner);
}

bool ComponentTransform2D::Update(float dt)
{
	float zoomRatio = app->sceneManager->GetCurrentScene()->mainCamera->GetZoomRatio();
	float4 viewport = app->editor->GetGameView()->GetBounds();

	if (firstTime) 
	{
		lastViewportBounds.w = app->editor->GetGameView()->GetBounds().w;
		lastViewportBounds.z = app->editor->GetGameView()->GetBounds().z;
		firstTime = false;
	}
	
	if (lastViewportBounds.w != viewport.w)
	{
		//position.y +=(viewport.w - lastViewportBounds.w)/100;
		
	}
	else if (lastViewportBounds.z != viewport.z) 
	{
			position.x -=( viewport.z - lastViewportBounds.z)/3.5;
		
	}
	
	internalPosition.x = ((position.x)/24) / zoomRatio;
	internalPosition.y = (((position.y)/24)+0.5) / zoomRatio;	
	internalPosition.z = position.z;

	lastViewportBounds = viewport;
	scale.x = (((buttonWidth - 130) * (viewport.z / 25)) / viewport.z) / zoomRatio;
	scale.y = (((buttonHeight - 50) * (viewport.w / 23)) / viewport.w) / zoomRatio;

	/*internalPosition.x = position.x/1;
	internalPosition.y = position.y/8;*/

	rotationQuat = FromEulerToQuat(rotationEuler);

	float4x4 aux;
	transMatrix = aux.FromTRS(internalPosition, rotationQuat, scale);
	transmat = transMatrix;
	transMatrix = transMatrix.Transposed();

	return true;
}

void ComponentTransform2D::OnEditor()
{
	if (showEdit) {
		if (ImGui::CollapsingHeader("2DTransformComponent"))
		{
			ImGui::TextColored(ImVec4(0, 0, 255, 255), "Size");
			ImGui::DragFloat("Width", &buttonWidth, 0.5f, 0, 1000000);
			ImGui::DragFloat("Height", &buttonHeight, 0.5f, 0, 1000000);

			ImGui::TextColored(ImVec4(0, 0, 255, 255), "Position");

			ImGui::DragFloat("Position X", &position.x, 0.5f);
			ImGui::DragFloat("Position Y", &position.y, 0.5f);
			ImGui::DragFloat("Position Z", &position.z, 0.5f);
		}
	}
}


Quat ComponentTransform2D::FromEulerToQuat(float3 eulerAngles)
{
	eulerAngles.x = math::DegToRad(eulerAngles.x);
	eulerAngles.y = math::DegToRad(eulerAngles.y);
	eulerAngles.z = math::DegToRad(eulerAngles.z);

	return Quat::FromEulerXYZ(eulerAngles.x, eulerAngles.y, eulerAngles.z);
}

float3 ComponentTransform2D::FromQuatToEuler(Quat quatAngles)
{
	float3 angles = quatAngles.ToEulerXYZ();
	angles.x = math::RadToDeg(angles.x);
	angles.y = math::RadToDeg(angles.y);
	angles.z = math::RadToDeg(angles.z);

	return float3(angles.x, angles.y, angles.z);
}
bool ComponentTransform2D::OnLoad(JsonParsing& node)
{
	active = node.GetJsonBool("Active");
	position = node.GetJson3Number(node, "Position");
	scale = node.GetJson3Number(node, "Scale");
	rotationEuler = node.GetJson3Number(node, "rotationEuler");
	buttonWidth = node.GetJsonNumber("buttonWidth");
	buttonHeight = node.GetJsonNumber("buttonHeight");

	transMatrix = float4x4::FromTRS(position, rotationQuat, scale);
	transmat = transMatrix;
	transMatrix = transMatrix.Transposed();
	return true;
}

bool ComponentTransform2D::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)18);
	file.SetNewJson3Number(file, "Position", position);
	file.SetNewJson3Number(file, "Scale", scale);
	file.SetNewJson3Number(file, "rotationEuler", rotationEuler);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "buttonWidth", buttonWidth);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "buttonHeight", buttonHeight);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}
