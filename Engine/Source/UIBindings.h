#pragma once

#include "ScriptComponent.h"

#include "ButtonComponent.h"
#include "SliderComponent.h"
#include "CheckBoxComponent.h"
#include "Transform2DComponent.h"

#include <metadata\object-forward.h>
#include <metadata\object.h>
#include <metadata/class.h>

// Button ==================================================
void UIFunctionButton(MonoObject* go)
{

}

// Checkbox ==================================================
void UIFunctionCheckbox(MonoObject* go)
{

}

// Slider ==================================================
void UIFunctionSlider(MonoObject* go)
{

}

// Transform2D ==================================================
void UIFunctionTransform2D(MonoObject* go)
{

}

MonoObject* Get2DPosition(MonoObject* go)
{
	ComponentTransform2D* tr = GetComponentMono<ComponentTransform2D*>(go);
	//float3 position = ;
	MonoObject* vec = app->moduleMono->Float3ToCS(tr->position);
	char* name = mono_string_to_utf8(mono_object_to_string(vec, 0));

	return vec;
}

void Set2DPosition(MonoObject* go, MonoObject* position)
{
	if (ComponentTransform2D* tr = GetComponentMono<ComponentTransform2D*>(go))
	{
		int i = 0;
		/*tr->SetPosition(app->moduleMono->UnboxVector(position));
		tr->UpdateTransform();*/
	}
}