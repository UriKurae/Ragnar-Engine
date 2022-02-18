#include "Application.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"

#include "LightComponent.h"

#include "imgui/imgui.h"

ComponentLight::ComponentLight()
{
	this->type = ComponentType::LIGHT;
}

ComponentLight::~ComponentLight()
{
	switch (light->type)
	{
		case LightType::DIRECTIONAL:
		{
			//delete app->renderer3D->dirLight;
			//app->renderer3D->dirLight = 0;

			break;
		}

		case LightType::POINT:
		{
			//app->renderer3D->DeletePointLight((PointLight*)light);
	
			break;
		}
	}
}

bool ComponentLight::Update(float dt)
{
	if (light->type == LightType::DIRECTIONAL)
	{
		DirectionalLight* l = (DirectionalLight*)light;
				
		if (TransformComponent* tr = owner->GetComponent<TransformComponent>())
		{
			l->dir = tr->GetRotation().CastToFloat4().Float3Part();
			l->dir.Normalize();
		}
	}
	else if (light->type == LightType::POINT)
	{
		if (TransformComponent* tr = owner->GetComponent<TransformComponent>())
		{
			PointLight* l = (PointLight*)light;
			if (tr->GetPosition().x != l->position.x || tr->GetPosition().y != l->position.y || tr->GetPosition().z != l->position.z)
			{
				l->position = tr->GetPosition();
				light = l;
			}
		}
	}
	else if (light->type == LightType::SPOT)
	{
		if (TransformComponent* tr = owner->GetComponent<TransformComponent>())
		{
			SpotLight* l = (SpotLight*)light;
			if (tr->GetPosition().x != l->position.x || tr->GetPosition().y != l->position.y || tr->GetPosition().z != l->position.z)
			{
				l->position = tr->GetPosition();
				light = l;
			}
		}
	}

	return true;
}

void ComponentLight::OnEditor()
{
	switch (light->type)
	{
		case LightType::DIRECTIONAL:
		{
			if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Separator();
				ImGui::Dummy({ 10,10 });

				DirectionalLight* l = (DirectionalLight*)light;

				ImGui::ColorEdit3("Ambient Color", l->ambient.ptr());
				ImGui::ColorEdit3("Diffuse Color", l->diffuse.ptr());
				ImGui::ColorEdit3("Specular Color", l->specular.ptr());

				ImGui::Text("%f %f %f", l->dir.x, l->dir.y, l->dir.z);
			}

			break;
		}
		
		case LightType::POINT:
		{
			if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
			{
				PointLight* l = (PointLight*)light;

				ImGui::ColorEdit3("Ambient Color", l->ambient.ptr());
				ImGui::ColorEdit3("Diffuse Color", l->diffuse.ptr());
				ImGui::ColorEdit3("Specular Color", l->specular.ptr());

				ImGui::DragFloat("Intensity", &l->intensity, 0.1f, 0.0f, 100.0f);

				ImGui::DragFloat("Constant", &l->constant, 0.001f, 0.1f, 100.0f);
				ImGui::DragFloat("Linear", &l->lin, 0.001f);
				ImGui::DragFloat("Quadratic", &l->quadratic, 0.001f, 0.0f, 1.0f);

			}

			break;
		}

		case LightType::SPOT:
		{
			SpotLight* l = (SpotLight*)light;

			ImGui::ColorEdit3("Ambient Color", l->ambient.ptr());
			ImGui::ColorEdit3("Diffuse Color", l->diffuse.ptr());
			ImGui::ColorEdit3("Specular Color", l->specular.ptr());

			ImGui::DragFloat("Intensity", &l->intensity, 0.1f);
			ImGui::DragFloat("CutOff", &l->cutOff, 0.1f);
			ImGui::DragFloat("Outer CutOff", &l->outerCutOff, 0.1f);
		}
	}
}

void ComponentLight::SetLight(Light* light)
{
	this->light = light;
}

bool ComponentLight::OnLoad(JsonParsing& node)
{
	return true;
}

bool ComponentLight::OnSave(JsonParsing& node, JSON_Array* array)
{
	return true;
}

//JSON_Value* ComponentLight::Save()
//{
//	JSON_Value* value = json_value_init_object();
//	JSON_Object* obj = json_value_get_object(value);
//
//	json_object_set_number(obj, "Type", 5);
//	
//	if(light->type == LightType::DIRECTIONAL)
//		json_object_set_string(obj, "lightType", "directional");
//	else if (light->type == LightType::POINT)
//		json_object_set_string(obj, "lightType", "point");
//
//	switch (light->type)
//	{
//		case LightType::DIRECTIONAL:
//		{
//			DirectionalLight* l = (DirectionalLight*)light;
//			
//			json_object_dotset_number(obj, "dir.x", l->dir.x);
//			json_object_dotset_number(obj, "dir.y", l->dir.y);
//			json_object_dotset_number(obj, "dir.z", l->dir.z);
//
//			json_object_dotset_number(obj, "ambient.x", l->ambient.x);
//			json_object_dotset_number(obj, "ambient.y", l->ambient.y);
//			json_object_dotset_number(obj, "ambient.z", l->ambient.z);
//			
//			json_object_dotset_number(obj, "diffuse.x", l->diffuse.x);
//			json_object_dotset_number(obj, "diffuse.y", l->diffuse.y);
//			json_object_dotset_number(obj, "diffuse.z", l->diffuse.z);
//
//			json_object_dotset_number(obj, "specular.x", l->specular.x);
//			json_object_dotset_number(obj, "specular.y", l->specular.y);
//			json_object_dotset_number(obj, "specular.z", l->specular.z);
//
//			break;
//		}
//		case LightType::POINT:
//		{
//			PointLight* l = (PointLight*)light;
//
//			json_object_dotset_number(obj, "pos.x", l->position.x);
//			json_object_dotset_number(obj, "pos.y", l->position.y);
//			json_object_dotset_number(obj, "pos.z", l->position.z);
//
//			json_object_dotset_number(obj, "ambient.x", l->ambient.x);
//			json_object_dotset_number(obj, "ambient.y", l->ambient.y);
//			json_object_dotset_number(obj, "ambient.z", l->ambient.z);
//
//			json_object_dotset_number(obj, "diffuse.x", l->diffuse.x);
//			json_object_dotset_number(obj, "diffuse.y", l->diffuse.y);
//			json_object_dotset_number(obj, "diffuse.z", l->diffuse.z);
//
//			json_object_dotset_number(obj, "specular.x", l->specular.x);
//			json_object_dotset_number(obj, "specular.y", l->specular.y);
//			json_object_dotset_number(obj, "specular.z", l->specular.z);
//
//			json_object_set_number(obj, "constant", l->constant);
//			json_object_set_number(obj, "linear", l->lin);
//			json_object_set_number(obj, "quadratic", l->quadratic);
//
//
//			break;
//		}
//	}
//
//	return value;
//}
//
//void ComponentLight::Load(JSON_Object* obj, GameObject* parent)
//{
//	//light = new Light();
//
//	LightType type;
//
//	std::string lightType = json_object_get_string(obj, "lightType");
//	if (lightType == "directional")
//	{
//		type = LightType::DIRECTIONAL;
//		//light = app->renderer3D->dirLight;
//	}
//	else if (lightType == "point")
//		type = LightType::POINT;
//
//
//	switch (type)
//	{
//		case LightType::DIRECTIONAL:
//		{
//			float3 dir;
//			dir.x = json_object_dotget_number(obj, "dir.x");
//			dir.y = json_object_dotget_number(obj, "dir.y");
//			dir.z = json_object_dotget_number(obj, "dir.z");
//			
//			//app->renderer3D->dirLight->dir = dir;
//			//light = app->renderer3D->dirLight;
//
//			//l->ambient.x = json_object_dotget_number(obj, "ambient.x");
//			//l->ambient.y = json_object_dotget_number(obj, "ambient.y");
//			//l->ambient.z = json_object_dotget_number(obj, "ambient.z");
//			//
//			//l->diffuse.x = json_object_dotget_number(obj, "diffuse.x");
//			//l->diffuse.y = json_object_dotget_number(obj, "diffuse.y");
//			//l->diffuse.z = json_object_dotget_number(obj, "diffuse.z");
//			//
//			//l->specular.x = json_object_dotget_number(obj, "specular.x");
//			//l->specular.y = json_object_dotget_number(obj, "specular.y");
//			//l->specular.z = json_object_dotget_number(obj, "specular.z");
//
//			//app->renderer3D->goDirLight = new GameObject("Directional Light");
//			
//
//			break;
//		}
//		case LightType::POINT:
//		{
//			PointLight* l = new PointLight();
//
//			TransformComponent* tr = owner->GetComponent<TransformComponent>();
//			l->position = tr->GetPosition();
//
//			l->ambient.x = json_object_dotget_number(obj, "ambient.x");
//			l->ambient.y = json_object_dotget_number(obj, "ambient.y");
//			l->ambient.z = json_object_dotget_number(obj, "ambient.z");
//
//			l->diffuse.x = json_object_dotget_number(obj, "diffuse.x");
//			l->diffuse.y = json_object_dotget_number(obj, "diffuse.y");
//			l->diffuse.z = json_object_dotget_number(obj, "diffuse.z");
//
//			l->specular.x = json_object_dotget_number(obj, "specular.x");
//			l->specular.y = json_object_dotget_number(obj, "specular.y");
//			l->specular.z = json_object_dotget_number(obj, "specular.z");
//
//			l->constant = json_object_get_number(obj, "constant");
//			l->lin = json_object_get_number(obj, "linear");
//			l->quadratic = json_object_get_number(obj, "quadratic");
//
//
//			//app->renderer3D->AddPointLight(l);
//			light = l;
//
//			bool a;
//			a = true;
//			break;
//		}
//	}
//}
