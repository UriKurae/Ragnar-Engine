#include "Application.h"
#include "Transform2DComponent.h"
#include <math.h>
#include"CameraComponent.h"

#include "ModuleInput.h"


ComponentTransform2D::ComponentTransform2D(GameObject* theObject, float3 pos, float3 sca, float3 rot, GameObject* Own)
{
	//UID = GenerateUID();
	lastPosition = { 0,0,0 };
	lastRotation = { 0,0,0 };
	//type = ComponentType::TRANSFORM2D;

	position = pos;
	scale = sca;
	rotationEuler = rot;
	generalScale = 1.0f;

	buttonWidth = scale.x;
	buttonHeight = scale.y;

	rotationQuat = FromEulerToQuat(rotationEuler);

	float4x4 aux;
	transMatrix = aux.FromTRS(position, rotationQuat, scale);
	transmat = transMatrix;
	transMatrix = transMatrix.Transposed();
	own = Own;
	

	//ComponentType t = getComponentType();
	//CreateAABB(ComponentType::MESH, theObject, true);
}
ComponentTransform2D::ComponentTransform2D(float3 pos, float3 sca, float3 rot, GameObject* Own)
{
	//UID = GenerateUID();
	lastPosition = { 0,0,0 };
	lastRotation = { 0,0,0 };
	//type = ComponentType::TRANSFORM2D;
	own = Own;
	position = pos;
	scale = sca;
	rotationEuler = rot;
	generalScale = 1.0f;

	buttonWidth = scale.x;
	buttonHeight = scale.y;

	rotationQuat = FromEulerToQuat(rotationEuler);

	float4x4 aux;
	transMatrix = aux.FromTRS(position, rotationQuat, scale);
	transmat = transMatrix;
	transMatrix = transMatrix.Transposed();
	matrix = transMatrix.ptr();
	
	//name = "Transform2D Component";

	//CreateAABB(ComponentType::PLANE, App->scene->gameObjects[App->scene->gameObjects.size() - 1], true);
}

//ComponentType ComponentTransform2D::getComponentType()
//{
//	/*for (int b = 0; b < App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components.size(); b++)
//	{
//		if (App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components[b]->type == ComponentType::UI_BUTTON)
//		{
//			return ComponentType::UI_BUTTON;
//		}
//		else if (App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components[b]->type == ComponentType::UI_CHECKBOX)
//		{
//			return ComponentType::UI_CHECKBOX;
//		}
//		else if (App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components[b]->type == ComponentType::UI_INPUTBOX)
//		{
//			return ComponentType::UI_INPUTBOX;
//		}
//		else if (App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components[b]->type == ComponentType::UI_SLIDER)
//		{
//			return ComponentType::UI_SLIDER;
//		}
//		
//	}*/
//}
ComponentTransform2D::~ComponentTransform2D()
{
}

void ComponentTransform2D::Enable()
{
}

void ComponentTransform2D::Update(bool releaseMouse)
{
	rotationQuat = FromEulerToQuat(rotationEuler);

	float4x4 aux;
	transMatrix = aux.FromTRS(position, rotationQuat, scale);
	transmat = transMatrix;
	transMatrix = transMatrix.Transposed();
	matrix = transMatrix.ptr();

	

	float4x4 aux1;
	
	//for (int a = 0; a < own->GetChilds().size(); a++) {
	//	for (int b = 0; b < owner->childrens[a]->components.size(); b++) {
	//		
	//			//owner->childrens[a]->components[b]->scale += (scale - lastScale);
	//			owner->childrens[a]->components[b]->position += (position - lastPosition);
	//			//owner->childrens[a]->components[b]->rotationEuler += (rotationEuler - lastRotation);
	//			//owner->childrens[a]->components[b]->rotationQuat = FromEulerToQuat(owner->childrens[a]->components[b]->rotationEuler);

	//			owner->childrens[a]->components[b]->transmat = aux1.FromTRS(owner->childrens[a]->components[b]->position, owner->childrens[a]->components[b]->rotationQuat, owner->childrens[a]->components[b]->scale);
	//			owner->childrens[a]->components[b]->transmat = owner->childrens[a]->components[b]->transmat.Transposed();
	//			owner->childrens[a]->matrix = owner->childrens[a]->components[b]->transmat.ptr();


	//		
	//	}
	//}
	lastScale = scale;
	lastPosition = position;
	lastRotation = rotationEuler;
	lastGeneralScale = generalScale;
}
void ComponentTransform2D::setLast() 
{
	lastScale = scale;
	lastPosition = position;
	lastRotation = rotationEuler;
	lastGeneralScale = generalScale;
}
void ComponentTransform2D::Disable()
{
}

void ComponentTransform2D::OnEditor()
{
	lastScale = scale;
	lastPosition = position;
	lastRotation = rotationEuler;
	lastGeneralScale = generalScale;
	if (ImGui::CollapsingHeader("2D Transform"))
	{
		ImGui::TextColored(ImVec4(0, 0, 255, 255), "Size");
		if (ImGui::DragFloat("Width", &buttonWidth, 0.5f, 0, 1000000))
		{
			bool release = false;
			if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
				release = true;
			scale.x = buttonWidth;
			scale.y = buttonHeight;
			Update(release);
		}
		if (ImGui::DragFloat("Height", &buttonHeight, 0.5f, 0, 1000000))
		{
			bool release = false;
			if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
				release = true;
			scale.x = buttonWidth;
			scale.y = buttonHeight;
			Update(release);
		}

		ImGui::TextColored(ImVec4(0, 0, 255, 255), "Position");
		static bool wasNull = true;
		/*if (app->camera->GameCam != nullptr)
			wasNull = false;*/

		if (ImGui::DragFloat("Position X", &position.x, 0.5f))
		{
			bool release = false;
			if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
				release = true;
			Update(release);
		}
		if (ImGui::DragFloat("Position Y", &position.y, 0.5f))
		{
			bool release = false;
			if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
				release = true;
			Update(release);
		}
		if (ImGui::DragFloat("Position Z", &position.z, 0.5f))
		{
			bool release = false;
			if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
				release = true;
			Update(release);
		}

	}
	
	
}

//void ComponentTransform2D::Load(const char* path)
//{
//	//Reading JSON File
//	JSON_Value* userData = json_parse_file(path);
//
//	UID = json_object_get_number(json_object(userData), "UID");
//	position.x = json_object_get_number(json_object(userData), "Position X");
//	position.y = json_object_get_number(json_object(userData), "Position Y");
//	position.z = json_object_get_number(json_object(userData), "Position Z");
//	
//	scale.x = json_object_get_number(json_object(userData), "Scale X");
//	scale.y = json_object_get_number(json_object(userData), "Scale Y");
//	scale.z = json_object_get_number(json_object(userData), "Scale Z");
//	
//	rotationEuler.x = json_object_get_number(json_object(userData), "Rotation X");
//	rotationEuler.y = json_object_get_number(json_object(userData), "Rotation Y");
//	rotationEuler.z = json_object_get_number(json_object(userData), "Rotation Z");
//
//	App->editor->AddLog("Loaded Transform 2D Component Data\n");
//
//	Update(false);
//}
//
//void ComponentTransform2D::Save(const char* path)
//{
//	//Creating Json file
//	JSON_Value* user_data = json_value_init_object();
//
//	json_object_set_number(json_object(user_data), "UID", UID);
//	json_object_set_number(json_object(user_data), "Position X", position.x);
//	json_object_set_number(json_object(user_data), "Position Y", position.y);
//	json_object_set_number(json_object(user_data), "Position Z", position.z);
//	json_object_set_number(json_object(user_data), "Scale X", scale.x);
//	json_object_set_number(json_object(user_data), "Scale Y", scale.y);
//	json_object_set_number(json_object(user_data), "Scale Z", scale.z);
//	json_object_set_number(json_object(user_data), "Rotation X", rotationEuler.x);
//	json_object_set_number(json_object(user_data), "Rotation Y", rotationEuler.y);
//	json_object_set_number(json_object(user_data), "Rotation Z", rotationEuler.z);
//	
//
//	json_serialize_to_file_pretty(user_data, path);
//
//	App->editor->AddLog("Saved Transform 2D Component Data\n");
//}
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

