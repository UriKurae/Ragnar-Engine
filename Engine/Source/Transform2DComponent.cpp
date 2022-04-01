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

#define CONVERSION_FACTOR 45
ComponentTransform2D::ComponentTransform2D(/*float3 pos, float3 sca, float3 rot,*/ GameObject* own)
{
	internalPosition = { 0,0,0 };
	position = { 0,0,36.0f };
	scale.x = 30;
	scale.y = 15;
	scale.z = 1;

	lastPosition= position;
	lastScale.x=buttonWidth;
	lastScale.y = buttonHeight;
	//rotationEuler = rot;
	rotationEuler = float3(0, 0, 0);
	buttonWidth = 100;
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
void ComponentTransform2D::UpdateChilds(float3 newPosition, float2 newScale) {
	std::vector<GameObject*> auxiliar;
	auxiliar=owner->GetChilds();

	position += newPosition;
	buttonWidth += newScale.x;
	buttonHeight += newScale.y;
	for (int a = 0; a < auxiliar.size(); a++) 
	{		
		ComponentTransform2D* aux=auxiliar[a]->GetComponent<ComponentTransform2D>();
		aux->UpdateChilds(newPosition, newScale);
	}
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
		float change;
		change = lastViewportBounds.w - viewport.w;
		if ((lastViewportBounds.w / 2) + position.y < (lastViewportBounds.w / 2))
		{

			position.y += (change / 2);
		}
		else if ((lastViewportBounds.w / 2) + position.y > (lastViewportBounds.w / 2))
		{
			position.y -= (change / 2);
		}

		
	}
	if (lastViewportBounds.z != viewport.z) 
	{
		float change;
		change = lastViewportBounds.z-viewport.z;
		if ((lastViewportBounds.z /2)+position.x < (lastViewportBounds.z / 2))
		{
			
			position.x += (change/2);
		}
		else if((lastViewportBounds.z / 2)+position.x > (lastViewportBounds.z / 2))
		{
			position.x -= (change/2 );
		}		
		
			
	}
	lastViewportBounds = viewport;	
	
	internalPosition.x = (position.x/ CONVERSION_FACTOR) / zoomRatio;
	internalPosition.y = (position.y / CONVERSION_FACTOR) / zoomRatio;
	internalPosition.z = position.z;

	scale.x = (buttonWidth/ CONVERSION_FACTOR) / zoomRatio;
	scale.y = (buttonHeight / CONVERSION_FACTOR) / zoomRatio;
	

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
			if (position.x != lastPosition.x || position.y != lastPosition.y || position.z != lastPosition.z || scale.x != lastScale.x || scale.y != lastScale.y) 
			{
				float3 diferencePosition = position - lastPosition;
				float2 diferenceScale = { buttonWidth - lastScale.x ,buttonHeight - lastScale.y };
				UpdateChilds(diferencePosition, diferenceScale);
				position -= (diferencePosition);
				buttonWidth -= (diferenceScale.x);
				buttonHeight -= (diferenceScale.y);
				lastPosition = position;
				lastScale.x= buttonWidth;
				lastScale.y = buttonHeight;
			}
			if (lastZ != position.z) 
			{
				app->userInterface->OrderButtons();
			}
			lastZ = position.z;
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

	lastPosition = position;
	lastScale.x = buttonWidth;
	lastScale.y = buttonHeight;
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
