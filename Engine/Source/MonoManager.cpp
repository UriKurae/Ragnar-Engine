#include "Globals.h"
#include "Application.h"
#include "MonoManager.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "TransformComponent.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/class.h>
#include <mono/metadata/threads.h>

#include "GameObject.h"
#include "ScriptBindings.h"

#include <iostream>
#include <fstream>
#include <PugiXML/pugixml.hpp>
#include "FileSystem.h"
#include "ImGui/imgui.h"
#include "ModuleEditor.h"

MonoManager::MonoManager(bool start_enabled) : Module(start_enabled), domain(nullptr), domainThread(nullptr), assembly(nullptr), image(nullptr)
, jitDomain(nullptr)
{

}

MonoManager::~MonoManager()
{}

bool MonoManager::Init(JsonParsing& node)
{
	LOG(LogType::L_NORMAL, "Setting up the camera");
	bool ret = true;

//#ifndef STANDALONE
	CMDCompileCS();
//#endif // !STANDALONE

	mono_set_dirs("mono-runtime/lib", "mono-runtime/etc");
	mono_config_parse(NULL);
	jitDomain = mono_jit_init("myapp");

	mono_add_internal_call("RagnarEngine.Input::GetKey", GetKey);
	mono_add_internal_call("RagnarEngine.Input::GetMouseClick", GetMouseClick);
	mono_add_internal_call("RagnarEngine.Input::GetMouseX", MouseX);
	mono_add_internal_call("RagnarEngine.Input::GetMouseY", MouseY);

	mono_add_internal_call("RagnarEngine.Transform::get_localPosition", GetPosition);
	mono_add_internal_call("RagnarEngine.Transform::get_globalPosition", GetGlobalPosition);
	mono_add_internal_call("RagnarEngine.Transform::get_localRotation", GetRotation);
	mono_add_internal_call("RagnarEngine.Transform::get_globalRotation", GetGlobalRotation);
	mono_add_internal_call("RagnarEngine.Transform::get_scale", GetScale);
	mono_add_internal_call("RagnarEngine.Transform::GetUp", GetUp);
	mono_add_internal_call("RagnarEngine.Transform::GetRight", GetRight);
	mono_add_internal_call("RagnarEngine.Transform::GetForward", GetForward);
	
	mono_add_internal_call("RagnarEngine.Transform::set_localPosition", SetPosition);
	mono_add_internal_call("RagnarEngine.Transform::set_localRotation", SetRotation);
	mono_add_internal_call("RagnarEngine.Transform::set_scale", SetScale);

	mono_add_internal_call("RagnarEngine.RagnarComponent::get_gameObject", GetGameObjectMonoObject);
	mono_add_internal_call("RagnarEngine.InternalCalls::CreateGameObject", InstantiateGameObject);
	mono_add_internal_call("RagnarEngine.GameObject::TryGetComponent", TryGetComponentMono);

	mono_add_internal_call("RagnarEngine.Time::get_deltaTime", GetGameTimeStep);
	mono_add_internal_call("RagnarEngine.Debug::Log", LogMono);


	InitMono();

	return ret;
}

// -----------------------------------------------------------------
bool MonoManager::CleanUp()
{
	LOG(LogType::L_NORMAL, "Cleaning mono domain");

	//mono_domain_unload(domain);
	mono_jit_cleanup(jitDomain); //Mono cleanup

	return true;
}

//#ifndef STANDALONE
//void MonoManager::OnGUI()
//{
//	if (ImGui::CollapsingHeader("Mono Settings", ImGuiTreeNodeFlags_DefaultOpen))
//	{
//		ImGui::Text("Compile tool: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "MSBuild portable version");
//
//	}
//}
//#endif // !STANDALONE

void MonoManager::ReCompileCS()
{
	if (app->scene->GetGameState() == GameState::PLAYING)
		return;

	app->scene->SaveScene("Assets/Scenes/scenePlay.ragnar");

	//TODO: Clean scene and all render data
	//app->scene->CleanScene();
	//app->renderer3D->ClearAllRenderData();

	mono_domain_unload(domain);
	mono_thread_cleanup();

	while (mono_domain_is_unloading(domain) == true)
	{

	}

	CMDCompileCS();
	InitMono();

	app->scene->LoadScene("Assets/Scenes/scenePlay.ragnar");
	app->fs->RemoveFile("Assets/Scenes/scenePlay.ragnar");

}

//ASK: Is this the worst idea ever? TOO SLOW
float3 MonoManager::UnboxVector(MonoObject* _obj)
{
	float3 ret;

	MonoClass* klass = mono_object_get_class(_obj);
	mono_field_get_value(_obj, mono_class_get_field_from_name(klass, "x"), &ret.x);
	mono_field_get_value(_obj, mono_class_get_field_from_name(klass, "y"), &ret.y);
	mono_field_get_value(_obj, mono_class_get_field_from_name(klass, "z"), &ret.z);
	return ret;
}
//ASK: Is this the worst idea ever? TOO SLOW
Quat MonoManager::UnboxQuat(MonoObject* _obj)
{
	Quat ret;

	MonoClass* klass = mono_object_get_class(_obj);
	mono_field_get_value(_obj, mono_class_get_field_from_name(klass, "x"), &ret.x);
	mono_field_get_value(_obj, mono_class_get_field_from_name(klass, "y"), &ret.y);
	mono_field_get_value(_obj, mono_class_get_field_from_name(klass, "z"), &ret.z);
	mono_field_get_value(_obj, mono_class_get_field_from_name(klass, "w"), &ret.w);
	return ret;
}

void MonoManager::DebugAllFields(const char* className, std::vector<SerializedField>& _data, MonoObject* obj, ScriptComponent* script, const char* nameSpace)
{
	void* iter = NULL;
	MonoClassField* field;
	MonoClass* klass = mono_class_from_name(mono_assembly_get_image(app->moduleMono->assembly), nameSpace, className);
	while (field = mono_class_get_fields(klass, &iter))
	{
		if (mono_field_get_flags(field) != 1) //TODO: Hardcoded private = 1, public = 6, static = 22, wtf does this mean?
		{
			SerializedField pushField = SerializedField(field, obj, script);
			//uint32_t test = mono_field_get_flags(field);

			_data.push_back(pushField);
			//LOG(LogType::L_NORMAL, mono_field_full_name(method2));
		}
	}
}

void MonoManager::DebugAllMethods(const char* nsName, const char* className, std::vector<std::string>& _data)
{
	void* iter = NULL;
	MonoMethod* method2;
	MonoClass* klass = mono_class_from_name(mono_assembly_get_image(app->moduleMono->assembly), nsName, className);
	while (method2 = mono_class_get_methods(klass, &iter))
	{
		_data.push_back(mono_method_full_name(method2, 1));
		//LOG(LogType::L_NORMAL, mono_method_full_name(method2, 1));
	}
}

MonoObject* MonoManager::GoToCSGO(GameObject* inGo) const
{
	MonoClass* goClass = mono_class_from_name(image, SCRIPTS_NAMESPACE, "GameObject");
	uintptr_t goPtr = reinterpret_cast<uintptr_t>(inGo);

	void* args[3];
	args[0] = &inGo->name;
	args[1] = &goPtr;

	uintptr_t transPTR = reinterpret_cast<uintptr_t>(inGo->GetComponent<TransformComponent>());
	args[2] = &transPTR;


	MonoMethodDesc* constructorDesc = mono_method_desc_new("RagnarEngine.GameObject:.ctor(string,uintptr,uintptr)", true);
	MonoMethod* method = mono_method_desc_search_in_class(constructorDesc, goClass);
	MonoObject* goObj = mono_object_new(domain, goClass);
	mono_runtime_invoke(method, goObj, args, NULL);

	mono_method_desc_free(constructorDesc);

	return goObj;
}

MonoObject* MonoManager::Float3ToCS(float3& inVec) const
{

	MonoClass* vecClass = mono_class_from_name(image, SCRIPTS_NAMESPACE, "Vector3");

	MonoObject* vecObject = mono_object_new(domain, vecClass);
	const char* name = mono_class_get_name(mono_object_get_class(vecObject));

	void* args[3];
	args[0] = &inVec.x;
	args[1] = &inVec.y;
	args[2] = &inVec.z;

	MonoMethodDesc* constDesc = mono_method_desc_new("RagnarEngine.Vector3:.ctor(single,single,single)", true);
	MonoMethod* method = mono_method_desc_search_in_class(constDesc, vecClass);

	mono_runtime_invoke(method, vecObject, args, NULL);

	mono_method_desc_free(constDesc);
	return vecObject;
}

void MonoManager::LoadFieldData(SerializedField& _field, MonoObject* _object)
{
	switch (_field.type)
	{
	case MonoTypeEnum::MONO_TYPE_BOOLEAN:
		mono_field_get_value(_object, _field.field, &_field.fiValue.bValue);
		break;

	case MonoTypeEnum::MONO_TYPE_I4:
		mono_field_get_value(_object, _field.field, &_field.fiValue.iValue);
		break;

	case MonoTypeEnum::MONO_TYPE_CLASS:
		_field.fiValue.goValue = nullptr;
		break;

	case MonoTypeEnum::MONO_TYPE_GENERICINST:
	{
		/*const char* className = mono_type_get_name(mono_field_get_type(_field.field));

		int* data = nullptr;
		mono_field_get_value(_object, _field.field, &data);

		std::vector<int> test;
		while (data != nullptr)
		{
			test.push_back(*data);
			data++;
		}


		if (strcmp(className, "GameObject") == 0)
			_field.fiValue.goValue = nullptr;
		else if (strcmp(className, "GameObject") == 0)
			LOG(LogType::L_WARNING, "%s", className);*/
		break;
	}

	case MonoTypeEnum::MONO_TYPE_SZARRAY:
	{
		_field.fiValue.arrValue = nullptr;

		MonoArray* arr;
		mono_field_get_value(_object, _field.field, &_field.fiValue.arrValue);
		break;
	}

	case MonoTypeEnum::MONO_TYPE_R4:
		mono_field_get_value(_object, _field.field, &_field.fiValue.fValue);
		break;

	case MonoTypeEnum::MONO_TYPE_STRING:
		mono_field_get_value(_object, _field.field, &_field.fiValue.strValue);
		break;

	default:
		mono_field_get_value(_object, _field.field, &_field.fiValue.iValue);
		break;
	}
}

MonoObject* MonoManager::QuatToCS(Quat& inVec) const
{

	MonoClass* quadClass = mono_class_from_name(image, SCRIPTS_NAMESPACE, "Quaternion");
	MonoObject* quatObject = mono_object_new(domain, quadClass);

	void* args[4];
	args[0] = &inVec.x;
	args[1] = &inVec.y;
	args[2] = &inVec.z;
	args[3] = &inVec.w;

	MonoMethodDesc* constDesc = mono_method_desc_new("RagnarEngine.Quaternion:.ctor(single,single,single,single)", true);
	MonoMethod* method = mono_method_desc_search_in_class(constDesc, quadClass);

	mono_runtime_invoke(method, quatObject, args, NULL);

	mono_method_desc_free(constDesc);
	return quatObject;
}

GameObject* MonoManager::GameObject_From_CSGO(MonoObject* goObj)
{
	uintptr_t ptr = 0;
	MonoClass* goClass = mono_class_from_name(image, SCRIPTS_NAMESPACE, "GameObject");

	mono_field_get_value(goObj, mono_class_get_field_from_name(goClass, "pointer"), &ptr);

	return reinterpret_cast<GameObject*>(ptr);
}

GameObject* MonoManager::GameObject_From_CSCOMP(MonoObject* goComponent)
{
	uintptr_t ptr = 0;
	MonoClass* goClass = mono_class_from_name(image, SCRIPTS_NAMESPACE, "RagnarComponent");

	mono_field_get_value(goComponent, mono_class_get_field_from_name(goClass, "pointer"), &ptr);

	return reinterpret_cast<Component*>(ptr)->owner;
}

SerializedField::SerializedField() : field(nullptr), parentSC(nullptr)
{
	fiValue.iValue = 0;
}

SerializedField::SerializedField(MonoClassField* _field, MonoObject* _object, ScriptComponent* parent) : field(_field)
{
	type = static_cast<MonoTypeEnum>(mono_type_get_type(mono_field_get_type(field)));
	fiValue.iValue = 0;
	parentSC = parent;

	displayName += "##";
	displayName += mono_field_get_name(_field);

	MonoManager::LoadFieldData(*this, _object);
}

void MonoManager::CreateAssetsScript(const char* localPath)
{
	std::string unnormalizedPath("Assets/Scripts/");
	unnormalizedPath += localPath;
	app->fs->UnNormalizePath(unnormalizedPath);

	std::ofstream outfile(unnormalizedPath.c_str());

	std::string className("Assets/Scripts/");
	className += localPath;
	className = className.substr(className.find_last_of("/") + 1);
	className = className.substr(0, className.find_last_of("."));

	outfile << "using System;" << std::endl << "using RagnarEngine;" << std::endl << std::endl << "public class " << className.c_str() << " : RagnarComponent" << std::endl << "{" << std::endl <<
		"	public void Update()" << std::endl << "	{" << std::endl << std::endl << "	}" << std::endl << std::endl << "}";

	outfile.close();

	AddScriptToSLN(unnormalizedPath.c_str());
	ReCompileCS();
}

void MonoManager::AddScriptToSLN(const char* scriptLocalPath)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("Assembly-CSharp.csproj");

	if (result.status == pugi::xml_parse_status::status_file_not_found)
		assert(false, "XML File not loaded");

	std::string path;
	path += scriptLocalPath;
	std::string name = path.substr(path.find_last_of("\\"));

	pugi::xml_node whereToAdd = doc.child("Project");
	for (pugi::xml_node panel = whereToAdd.first_child(); panel != nullptr; panel = panel.next_sibling())
	{
		if (strcmp(panel.name(), "ItemGroup") == 0 && strcmp(panel.first_child().name(), "Compile") == 0)
		{
			panel = panel.append_child();
			panel.set_name("Compile");
			pugi::xml_attribute att = panel.append_attribute("Include");
			att.set_value(path.c_str());

			break;
		}
	}

	doc.save_file("Assembly-CSharp.csproj");
}

void MonoManager::RemoveScriptFromSLN(const char* scriptLocalPath)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("Assembly-CSharp.csproj");

	if (result.status == pugi::xml_parse_status::status_file_not_found)
		assert(false, "XML File not loaded");

	std::string path;

	pugi::xml_node whereToRemove = doc.child("Project");
	for (pugi::xml_node panel = whereToRemove.first_child(); panel != nullptr; panel = panel.next_sibling())
	{
		if (strcmp(panel.name(), "ItemGroup") == 0 && strcmp(panel.first_child().name(), "Compile") == 0)
		{
			for (pugi::xml_node toRemove = panel.first_child(); toRemove != nullptr; toRemove = toRemove.next_sibling())
			{
				path = toRemove.attribute("Include").as_string();
				app->fs->NormalizePath(path);

				if (strcmp(path.c_str(), scriptLocalPath) == 0)
				{
					panel.remove_child(toRemove);
					break;
				}
			}
		}
	}

	doc.save_file("Assembly-CSharp.csproj");
}


void MonoManager::InitMono()
{
	domain = mono_domain_create_appdomain("D1", NULL);
	mono_domain_set(domain, 0);
	domainThread = mono_thread_attach(domain);

	MonoImageOpenStatus sts;

	if (app->fs->Exists("Library/ScriptsAssembly/Assembly-CSharp.dll") == false)
		assert(false && "You need to install the 'Visual Studio .NET desktop development pack' to run the engine");

	assembly = mono_assembly_open("Library/ScriptsAssembly/Assembly-CSharp.dll", &sts);
	if (!assembly)
		LOG(LogType::L_ERROR, "ERROR");

	image = mono_assembly_get_image(assembly);

	const MonoTableInfo* table_info = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
	int rows = mono_table_info_get_rows(table_info);

	MonoClass* _class = nullptr;

	userScripts.clear();
	for (int i = 0; i < rows; i++)
	{
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
		const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

		if (name[0] != '<')
		{
			const char* name_space = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			_class = mono_class_from_name(image, name_space, name);

			if (_class != nullptr && strcmp(mono_class_get_namespace(_class), SCRIPTS_NAMESPACE) != 0 && !mono_class_is_enum(_class))
			{
				userScripts.push_back(_class);
				LOG(LogType::L_WARNING, "%s", mono_class_get_name(_class));
			}
		}
	}
}