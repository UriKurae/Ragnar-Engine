#pragma once

#include "Application.h"
#include "ModuleInput.h"
#include <metadata\object-forward.h>
#include <metadata\object.h>

//Input bindings ===============================================================================

int GetKey(MonoObject* x)
{
	if (app != nullptr)
		return app->input->GetKey(*(int*)mono_object_unbox(x));

	return 0;
}
int GetMouseClick(MonoObject* x)
{
	if (app != nullptr)
		return app->input->GetMouseButton(*(int*)mono_object_unbox(x));

	return 0;
}
int MouseX()
{
	if (app != nullptr)
		return app->input->GetMouseXMotion();

	return 0;
}
int MouseY()
{
	if (app != nullptr)
		return app->input->GetMouseYMotion();

	return 0;
}