#pragma once

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