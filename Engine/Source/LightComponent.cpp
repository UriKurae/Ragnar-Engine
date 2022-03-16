#include "LightComponent.h"
#include "Application.h"

#include "ModuleRenderer3D.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "Lights.h"

ComponentLight::ComponentLight()
{
	this->type = ComponentType::LIGHT;
}

ComponentLight::~ComponentLight()
{
	switch (light->type)
	{
		case LightType::POINT:
		{
			app->renderer3D->RemovePointLight((PointLight*)light);
			//delete (light);
			//light = nullptr;
		}
	}
}

bool ComponentLight::Update(float dt)
{
	if (light->type == LightType::DIRECTIONAL)
	{			
		if (TransformComponent* tr = owner->GetComponent<TransformComponent>())
		{
			DirectionalLight* l = (DirectionalLight*)light;
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
			
			float3 dir = tr->GetRotation().CastToFloat4().Float3Part();
			if (dir.x == 0 && dir.y == 0 && dir.z == 0)
			{
				l->direction = { 0,-1,0 };
			}
			else
			{
				l->direction = tr->GetRotation().CastToFloat4().Float3Part();
				l->direction.Normalize();
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
			if (ImGui::CollapsingHeader(ICON_FA_LIGHTBULB" Light"))
			{
				ImGui::Separator();
				ImGui::Dummy({ 10,10 });

				DirectionalLight* l = (DirectionalLight*)light;

				ImGui::ColorEdit3("Ambient Color", l->ambient.ptr());
				ImGui::ColorEdit3("Diffuse Color", l->diffuse.ptr());
				ImGui::ColorEdit3("Specular Color", l->specular.ptr());

				ImGui::Text("%f %f %f", l->dir.x, l->dir.y, l->dir.z);

				ComponentOptions(this);
				ImGui::Separator();
			}

			break;
		}
		
		case LightType::POINT:
		{
			if (ImGui::CollapsingHeader(ICON_FA_LIGHTBULB" Light"))
			{
				PointLight* l = (PointLight*)light;

				ImGui::ColorEdit3("Ambient Color", l->ambient.ptr());
				ImGui::ColorEdit3("Diffuse Color", l->diffuse.ptr());
				ImGui::ColorEdit3("Specular Color", l->specular.ptr());

				ImGui::DragFloat("Intensity", &l->intensity, 0.1f, 0.0f, 100.0f);

				ImGui::DragFloat("Constant", &l->constant, 0.001f, 0.1f, 100.0f);
				ImGui::DragFloat("Linear", &l->lin, 0.001f);
				ImGui::DragFloat("Quadratic", &l->quadratic, 0.001f, 0.0f, 1.0f);

				ComponentOptions(this);
				ImGui::Separator();
			}

			break;
		}

		case LightType::SPOT:
		{
			if (ImGui::CollapsingHeader(ICON_FA_LIGHTBULB" Light"))
			{
				SpotLight* l = (SpotLight*)light;

				ImGui::ColorEdit3("Ambient Color", l->ambient.ptr());
				ImGui::ColorEdit3("Diffuse Color", l->diffuse.ptr());
				ImGui::ColorEdit3("Specular Color", l->specular.ptr());

				ImGui::DragFloat("Intensity", &l->intensity, 0.01f);
				ImGui::DragFloat("CutOff", &l->cutOff, 0.001f, 1.0f,0.0f);
				ImGui::DragFloat("Outer CutOff", &l->outerCutOff, 0.001f, 1.0f, 0.0f);

				ComponentOptions(this);
				ImGui::Separator();
			}
			break;
		}
	}
}

void ComponentLight::SetLight(Light* light)
{
	this->light = light;
}

bool ComponentLight::OnLoad(JsonParsing& node)
{
	active = node.GetJsonBool("Active");

	int lightType = node.GetJsonNumber("Light Type");

	switch ((LightType)lightType)
	{
		case LightType::DIRECTIONAL:
		{
			DirectionalLight* l = new DirectionalLight();

			l->dir = node.GetJson3Number(node, "Direction");
			l->ambient = node.GetJson3Number(node, "Ambient");
			l->diffuse = node.GetJson3Number(node, "Diffuse");
			l->specular = node.GetJson3Number(node, "Specular");

			light = l;

			delete app->renderer3D->dirLight;
			app->renderer3D->dirLight = l;

			break;
		}

		case LightType::POINT:
		{
			PointLight* l = new PointLight();

			l->position = node.GetJson3Number(node, "Position");
			l->ambient = node.GetJson3Number(node, "Ambient");
			l->diffuse = node.GetJson3Number(node, "Diffuse");
			l->specular = node.GetJson3Number(node, "Specular");
			l->constant = node.GetJsonNumber("Constant");
			l->lin = node.GetJsonNumber("Linear");
			l->quadratic = node.GetJsonNumber("Quadratic");

			light = l;
			app->renderer3D->AddPointLight(l);

			break;
		}

		case LightType::SPOT:
		{
			SpotLight* l = new SpotLight();

			l->position = node.GetJson3Number(node, "Position");
			l->direction = node.GetJson3Number(node, "Direction");
			l->ambient = node.GetJson3Number(node, "Ambient");
			l->diffuse = node.GetJson3Number(node, "Diffuse");
			l->specular = node.GetJson3Number(node, "Specular");
			l->cutOff = node.GetJsonNumber("CutOff");
			l->outerCutOff = node.GetJsonNumber("Outer CutOff");

			light = l;
			app->renderer3D->AddSpotLight(l);

			break;
		}
	}

	return true;
}

bool ComponentLight::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Light Type", (int)light->type);

	switch (light->type)
	{
		case LightType::DIRECTIONAL:
		{
			DirectionalLight* l = (DirectionalLight*)light;

			file.SetNewJson3Number(file, "Direction", l->dir);
			file.SetNewJson3Number(file, "Ambient", l->ambient);
			file.SetNewJson3Number(file, "Diffuse", l->diffuse);
			file.SetNewJson3Number(file, "Specular", l->specular);

			break;
		}
		case LightType::POINT:
		{
			PointLight* l = (PointLight*)light;
			file.SetNewJson3Number(file, "Position", l->position);
			file.SetNewJson3Number(file, "Ambient", l->ambient);
			file.SetNewJson3Number(file, "Diffuse", l->diffuse);
			file.SetNewJson3Number(file, "Specular", l->specular);
			file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Constant", l->constant);
			file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Linear", l->lin);
			file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Quadratic", l->quadratic);

			break;
		}

		case LightType::SPOT:
		{
			SpotLight* l = (SpotLight*)light;

			file.SetNewJson3Number(file, "Position", l->position);
			file.SetNewJson3Number(file, "Direction", l->direction);
			file.SetNewJson3Number(file, "Ambient", l->ambient);
			file.SetNewJson3Number(file, "Diffuse", l->diffuse);
			file.SetNewJson3Number(file, "Specular", l->specular);
			file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "CutOff", l->cutOff);
			file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Outer CutOff", l->outerCutOff);

			break;
		}

	}

	node.SetValueToArray(array, file.GetRootValue());
	
	return true;
}
