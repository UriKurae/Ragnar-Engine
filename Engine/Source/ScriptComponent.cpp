#include "ScriptComponent.h"
#include "ImGui/imgui.h"

#include "ModuleEditor.h"
#include "ModuleScene.h"

#include "GameObject.h"
#include "Application.h"
//#include "DETime.h"
#include "TransformComponent.h"

#include"JsonParsing.h"
#include <mono/metadata/class.h>
#include <mono/metadata/object.h>
#include <mono/metadata/object-forward.h>
#include <mono/metadata/debug-helpers.h>

ScriptComponent* ScriptComponent::runningScript = nullptr;
ScriptComponent::ScriptComponent(GameObject* own, const char* scriptName)
{
	type = ComponentType::SCRIPT;
	owner = own;

	LoadScriptData(scriptName);
}

ScriptComponent::~ScriptComponent()
{
	if (ScriptComponent::runningScript == this)
		ScriptComponent::runningScript = nullptr;

	mono_gchandle_free(noGCobject);

	for (unsigned int i = 0; i < fields.size(); i++)
	{
		if (fields[i].type == MonoTypeEnum::MONO_TYPE_CLASS && fields[i].fiValue.goValue != nullptr && fields[i].fiValue.goValue->csReferences.size() != 0)
		{
			//fields[i].fiValue.goValue->csReferences.erase(std::find(fields[i].fiValue.goValue->csReferences.begin(), fields[i].fiValue.goValue->csReferences.end(), &fields[i]));
			auto ptr = std::find(fields[i].fiValue.goValue->csReferences.begin(), fields[i].fiValue.goValue->csReferences.end(), &fields[i]);
			if (ptr != fields[i].fiValue.goValue->csReferences.end())
				fields[i].fiValue.goValue->csReferences.erase(ptr);
		}
	}

	methods.clear();
	fields.clear();
}


bool ScriptComponent::Update(float dt)
{
	if (app->scene->GetGameState() == GameState::NOT_PLAYING || app->scene->GetGameState() == GameState::PAUSE || updateMethod == nullptr)
		return false;

	ScriptComponent::runningScript = this; // I really think this is the peak of stupid code, but hey, it works, slow as hell but works.

	MonoObject* exec = nullptr;
	mono_runtime_invoke(updateMethod, mono_gchandle_get_target(noGCobject), NULL, &exec);

	if (exec != nullptr)
	{
		if (strcmp(mono_class_get_name(mono_object_get_class(exec)), "NullReferenceException") == 0)
		{
			LOG(LogType::L_ERROR, "Null reference exception detected");
		}
		else
		{
			LOG(LogType::L_ERROR, mono_class_get_name(mono_object_get_class(exec)));
		}
	}
	return true;
}

//#ifndef STANDALONE
void ScriptComponent::OnEditor()
{
	//if (Component::OnEditor() == true)
	//{
		//ImGui::Separator();
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Script"))
	{
		for (int i = 0; i < fields.size(); i++)
		{
			DropField(fields[i], "_GAMEOBJECT");
		}
		ImGui::Separator();
		for (int i = 0; i < methods.size(); i++)
		{
			ImGui::Text(methods[i].c_str());
		}
		ImGui::Separator();
	}
	ImGui::PopID();
	//}
}

void ScriptComponent::DisplayField(SerializedField& field, const char* dropType)
{
	switch (field.type)
	{
	case MonoTypeEnum::MONO_TYPE_BOOLEAN:
		mono_field_get_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.bValue);
		if (ImGui::Checkbox(field.displayName.c_str(), &field.fiValue.bValue))
			mono_field_set_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.bValue);
		break;

	case MonoTypeEnum::MONO_TYPE_I4:
		mono_field_get_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.iValue);
		if (ImGui::InputInt(field.displayName.c_str(), &field.fiValue.iValue, 1, 10))
			mono_field_set_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.iValue);
		break;

	case MonoTypeEnum::MONO_TYPE_CLASS:

		if (strcmp(mono_type_get_name(mono_field_get_type(field.field)), "DiamondEngine.GameObject") != 0)
		{
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "The class %s can't be serialized yet", mono_type_get_name(mono_field_get_type(field.field)));
			break;
		}

		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), (field.fiValue.goValue != nullptr) ? field.fiValue.goValue->name.c_str() : "None");
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dropType))
			{
				//field.fiValue.goValue = app->editor->GetDraggingGO();
				SetField(field.field, field.fiValue.goValue);
			}
			ImGui::EndDragDropTarget();
		}
		break;

	case MonoTypeEnum::MONO_TYPE_R4: {
		//float test = 0.f;
		mono_field_get_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.fValue);
		if (ImGui::InputFloat(field.displayName.c_str(), &field.fiValue.fValue, 0.1f))
			mono_field_set_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.fValue);
		break;
	}

	case MonoTypeEnum::MONO_TYPE_SZARRAY:
	{
		if (field.fiValue.arrValue == nullptr)
			break;

		MonoArrayType* test = mono_type_get_array_type(mono_field_get_type(field.field));

		const char* name = mono_class_get_name(test->eklass);
		MonoTypeEnum _type = static_cast<MonoTypeEnum>(mono_type_get_type(mono_class_get_type(test->eklass)));

		ImGui::NewLine();
		for (size_t i = 0; i < mono_array_length(field.fiValue.arrValue); i++)
		{
			switch (_type)
			{

			case MonoTypeEnum::MONO_TYPE_BOOLEAN:
				ImGui::Checkbox(std::to_string(i).c_str(), mono_array_addr(field.fiValue.arrValue, bool, i));
				break;

			case MonoTypeEnum::MONO_TYPE_I4:
				ImGui::InputInt(std::to_string(i).c_str(), mono_array_addr(field.fiValue.arrValue, int, i));
				break;

			case MonoTypeEnum::MONO_TYPE_CLASS:
			{
				if (strcmp(name, "GameObject") != 0)
				{
					ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "The array element can't be serialized yet");
					break;
				}

				MonoObject* arrayElementGO = mono_array_addr(field.fiValue.arrValue, MonoObject, i);
				//int hash = mono_object_hash(arrayElementGO);
				//if (arrayElementGO->synchronisation == nullptr && arrayElementGO->vtable == nullptr)
				//{
				//	ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "None");
				//	break;
				//}
				//MonoClass* cls = mono_object(arrayElementGO);
				GameObject* cpp_obj = app->moduleMono->GameObject_From_CSGO(arrayElementGO);
				ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), (cpp_obj == nullptr) ? "None" : cpp_obj->name.c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dropType))
					{
						//cpp_obj = app->editor->GetDraggingGO();
						arrayElementGO = app->moduleMono->GoToCSGO(cpp_obj);
						mono_array_set(field.fiValue.arrValue, MonoObject*, i, arrayElementGO);
					}
					ImGui::EndDragDropTarget();
				}

				break;
			}

			case MonoTypeEnum::MONO_TYPE_R4:
				ImGui::InputFloat(std::to_string(i).c_str(), mono_array_addr(field.fiValue.arrValue, float, i), 0.01f, 0.5f);
				break;

				//Array inside array? maybe another time when i learn how to structure this shit without a 50 case switch

				//case MonoTypeEnum::MONO_TYPE_STRING:
				//	ImGui::InputText(std::to_string(i).c_str(), mono_array_addr(field.fiValue.arrValue, char, i), 256);
				//	break;


			default:
				ImGui::Text("Ya mate, this class can't be serialized until I learn to code :D");
				break;
			}

		}

		break;
	}

	case MonoTypeEnum::MONO_TYPE_STRING:
	{

		MonoString* str = nullptr;
		mono_field_get_value(mono_gchandle_get_target(noGCobject), field.field, &str);

		char* value = mono_string_to_utf8(str);
		strcpy(field.fiValue.strValue, value);
		mono_free(value);

		if (ImGui::InputText(field.displayName.c_str(), &field.fiValue.strValue[0], 50))
		{
			str = mono_string_new(app->moduleMono->domain, field.fiValue.strValue);
			mono_field_set_value(mono_gchandle_get_target(noGCobject), field.field, str);
			//mono_free(str);
		}
		break;
	}

	default:
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), mono_type_get_name(mono_field_get_type(field.field)));
		break;
	}
}

void ScriptComponent::DropField(SerializedField& field, const char* dropType)
{

	const char* fieldName = mono_field_get_name(field.field);
	ImGui::PushID(fieldName);

	ImGui::Text(fieldName);
	ImGui::SameLine();

	DisplayField(field, dropType);

	ImGui::PopID();
}
//#endif


bool ScriptComponent::OnLoad(JsonParsing& nObj)
{
	Component::OnLoad(nObj);

	SerializedField* _field = nullptr;
	for (int i = 0; i < fields.size(); i++) //TODO IMPORTANT ASK: There must be a better way to do this... too much use of switches with this stuff, look at MONOMANAGER
	{
		_field = &fields[i];

		switch (_field->type)
		{
		case MonoTypeEnum::MONO_TYPE_BOOLEAN:
			_field->fiValue.bValue = nObj.GetJsonBool(mono_field_get_name(_field->field));
			mono_field_set_value(mono_gchandle_get_target(noGCobject), _field->field, &_field->fiValue.bValue);
			break;

		case MonoTypeEnum::MONO_TYPE_I4:
			_field->fiValue.iValue = nObj.GetJsonNumber(mono_field_get_name(_field->field));
			mono_field_set_value(mono_gchandle_get_target(noGCobject), _field->field, &_field->fiValue.iValue);
			break;

		case MonoTypeEnum::MONO_TYPE_CLASS:
		{
			if (strcmp(mono_type_get_name(mono_field_get_type(_field->field)), "DiamondEngine.GameObject") == 0)
				app->scene->referenceMap.emplace(nObj.GetJsonNumber(mono_field_get_name(_field->field)), _field);

			break;
		}

		case MonoTypeEnum::MONO_TYPE_SZARRAY:
			break;

		case MonoTypeEnum::MONO_TYPE_R4:
			_field->fiValue.fValue = nObj.GetJsonNumber(mono_field_get_name(_field->field));
			mono_field_set_value(mono_gchandle_get_target(noGCobject), _field->field, &_field->fiValue.fValue);
			break;

		case MonoTypeEnum::MONO_TYPE_STRING:
		{
			const char* ret = nObj.GetJsonString(mono_field_get_name(_field->field));

			if (ret == NULL)
				ret = "\0";

			strcpy(&_field->fiValue.strValue[0], ret);

			MonoString* str = mono_string_new(app->moduleMono->domain, _field->fiValue.strValue);
			mono_field_set_value(mono_gchandle_get_target(noGCobject), _field->field, str);
			break;
		}

		default:
			_field->fiValue.iValue = nObj.GetJsonNumber(mono_field_get_name(_field->field));
			mono_field_set_value(mono_gchandle_get_target(noGCobject), _field->field, &_field->fiValue.iValue);
			break;
		}
	}

	return true;
}


bool ScriptComponent::OnSave(JSON_Object* nObj)
{
	//Component::OnLoad(nObj);

	JsonParsing file = JsonParsing();
	//file.SetNewJsonNumber(nObj, "ScriptName", name.c_str());

	for (int i = 0; i < fields.size(); i++)
	{
		switch (fields[i].type)
		{
		case MonoTypeEnum::MONO_TYPE_BOOLEAN:
			mono_field_get_value(mono_gchandle_get_target(noGCobject), fields[i].field, &fields[i].fiValue.bValue);
			file.SetNewJsonBool(nObj, mono_field_get_name(fields[i].field), fields[i].fiValue.bValue);
			break;

		case MonoTypeEnum::MONO_TYPE_I4:
			mono_field_get_value(mono_gchandle_get_target(noGCobject), fields[i].field, &fields[i].fiValue.iValue);
			file.SetNewJsonNumber(nObj, mono_field_get_name(fields[i].field), fields[i].fiValue.iValue);
			break;

		case MonoTypeEnum::MONO_TYPE_CLASS:
			if (fields[i].fiValue.goValue != nullptr)
				file.SetNewJsonNumber(nObj, mono_field_get_name(fields[i].field), fields[i].fiValue.goValue->GetUUID());
			break;

		case MonoTypeEnum::MONO_TYPE_R4:
			mono_field_get_value(mono_gchandle_get_target(noGCobject), fields[i].field, &fields[i].fiValue.fValue);
			file.SetNewJsonNumber(nObj, mono_field_get_name(fields[i].field), fields[i].fiValue.fValue);
			break;

		case MonoTypeEnum::MONO_TYPE_STRING:
			file.SetNewJsonString(nObj, mono_field_get_name(fields[i].field), fields[i].fiValue.strValue);
			break;

		default:
			file.SetNewJsonNumber(nObj, mono_field_get_name(fields[i].field), fields[i].fiValue.iValue);
			break;
		}
	}
	return true;
}

void ScriptComponent::LoadScriptData(const char* scriptName)
{
	methods.clear();
	fields.clear();


	MonoClass* klass = mono_class_from_name(app->moduleMono->image, USER_SCRIPTS_NAMESPACE, scriptName);

	if (klass == nullptr)
	{
		LOG(LogType::L_ERROR, "Script %s was deleted and can't be loaded", scriptName);
		//name = "Missing script reference";
		return;
	}

	app->moduleMono->DebugAllMethods(USER_SCRIPTS_NAMESPACE, scriptName, methods);

	noGCobject = mono_gchandle_new(mono_object_new(app->moduleMono->domain, klass), false);
	mono_runtime_object_init(mono_gchandle_get_target(noGCobject));


	MonoClass* goClass = mono_object_get_class(mono_gchandle_get_target(noGCobject));
	uintptr_t ptr = reinterpret_cast<uintptr_t>(this);
	mono_field_set_value(mono_gchandle_get_target(noGCobject), mono_class_get_field_from_name(goClass, "pointer"), &ptr);

	MonoMethodDesc* mdesc = mono_method_desc_new(":Update", false);
	updateMethod = mono_method_desc_search_in_class(mdesc, klass);
	mono_method_desc_free(mdesc);

	MonoClass* baseClass = mono_class_get_parent(klass);
	if (baseClass != nullptr)
		app->moduleMono->DebugAllFields(mono_class_get_name(baseClass), fields, mono_gchandle_get_target(noGCobject), this, mono_class_get_namespace(baseClass));

	app->moduleMono->DebugAllFields(scriptName, fields, mono_gchandle_get_target(noGCobject), this, mono_class_get_namespace(goClass));
}

void ScriptComponent::SetField(MonoClassField* field, GameObject* value)
{
	mono_field_set_value(mono_gchandle_get_target(noGCobject), field, app->moduleMono->GoToCSGO(value));
}
