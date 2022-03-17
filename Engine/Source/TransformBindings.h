#pragma once

#include "ScriptComponent.h"

#include <metadata\object-forward.h>
#include <metadata\object.h>
#include <metadata/class.h>

template <typename T>
T GetComponentMono(MonoObject* go)
{
	MonoClass* goObj = mono_object_get_class(go);
	const char* name = mono_class_get_name(goObj);
	uint data = 0;
	mono_field_get_value(go, mono_class_get_field_from_name(goObj, "pointer"), &data);

	return reinterpret_cast<T>(data);
}

GameObject* GetGameObjectMono(MonoObject* component)
{
	uintptr_t ptr = 0;
	MonoClass* goClass = mono_object_get_class(component);
	const char* name = mono_class_get_name(goClass);
	mono_field_get_value(component, mono_class_get_field_from_name(goClass, "pointer"), &ptr);

	return reinterpret_cast<Component*>(ptr)->owner;
}

MonoObject* GetGameObjectMonoObject(MonoObject* comp)
{
	MonoObject* ret = app->moduleMono->GoToCSGO(GetGameObjectMono(comp));
	return ret;
}

MonoObject* TryGetComponentMono(MonoObject* go, MonoString* type, int inputType)
{
	GameObject* gameObject = app->moduleMono->GameObjectFromCSGO(go);

	ComponentType t = ComponentType(inputType);
	Component* comp = gameObject->GetComponent(t);

	if(t == ComponentType::SCRIPT)
		return mono_gchandle_get_target(dynamic_cast<ScriptComponent*>(comp)->noGCobject);

	MonoClass* cmpClass = mono_object_get_class(go);
	MonoObject* ret = mono_object_new(app->moduleMono->domain, cmpClass);

	MonoClassField* field = mono_class_get_field_from_name(cmpClass, "pointer");

	uintptr_t data = reinterpret_cast<uintptr_t>(comp);
	mono_field_set_value(ret, field, &data);

	return ret;
}

MonoArray* TryGetComponentsMono(MonoObject* go, MonoString* type, int inputType)
{
	GameObject* gameObject = app->moduleMono->GameObjectFromCSGO(go);

	std::vector<MonoObject*> objs;
	for (auto& comp : gameObject->components)
	{
		if (comp->type == ComponentType(inputType))
			objs.emplace_back(TryGetComponentMono(go, type, inputType));
	}

	MonoClass* goClass = mono_class_from_name(app->moduleMono->image, SCRIPTS_NAMESPACE, "GameObject");
	MonoArray* ret = mono_array_new(app->moduleMono->domain, goClass, objs.size());
	
	for (int i = 0; i < objs.size(); ++i)
		mono_array_set(ret, MonoObject*, i, objs[i]);

	return ret;
}

void LogMono(MonoString* x)
{
	if (x == NULL)
		return;

	char* msg = mono_string_to_utf8(x);
	DEBUG_LOG("Script LOG: %s", msg);
	//LOG(LogType::L_WARNING, msg);
	mono_free(msg);
}