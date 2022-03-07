#pragma once

#include "Application.h"
#include "ModuleInput.h"
#include "TransformBindings.h"

#include <metadata\object-forward.h>
#include <metadata\object.h>

// Input bindings ===============================================================================

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

// Input bindings ===============================================================================

// Component bindings ===============================================================================

// Transform ========

MonoObject* GetForward(MonoObject* go)
{
	TransformComponent* tr = GetComponentMono<TransformComponent*>(go);
	MonoClass* vecBind = mono_class_from_name(app->moduleMono->image, "RagnarEngine", "Vector3");
	return app->moduleMono->Float3ToCS(tr->GetForward());
}

MonoObject* GetRight(MonoObject* go)
{
	TransformComponent* tr = GetComponentMono<TransformComponent*>(go);
	MonoClass* vecBind = mono_class_from_name(app->moduleMono->image, "RagnarEngine", "Vector3");
	return app->moduleMono->Float3ToCS(tr->GetRight());
}

MonoObject* GetUp(MonoObject* go)
{
	TransformComponent* tr = GetComponentMono<TransformComponent*>(go);
	MonoClass* vecBind = mono_class_from_name(app->moduleMono->image, "RagnarEngine", "Vector3");
	return app->moduleMono->Float3ToCS(tr->GetUp());
}

MonoObject* GetPosition(MonoObject* go)
{
	TransformComponent* tr = GetComponentMono<TransformComponent*>(go);
	float3 position = tr->GetLocalTransform().TranslatePart();
	return app->moduleMono->Float3ToCS(position);
}

MonoObject* GetGlobalPosition(MonoObject* go)
{
	TransformComponent* tr = GetComponentMono<TransformComponent*>(go);
	float3 position = tr->GetGlobalTransform().TranslatePart();
	return app->moduleMono->Float3ToCS(position);
}

MonoObject* GetRotation(MonoObject* go)
{
	TransformComponent* tr = GetComponentMono<TransformComponent*>(go);
	Quat rotation = Quat::identity;
	float3 p, s;
	tr->GetLocalTransform().Decompose(p, rotation, s);
	return app->moduleMono->QuatToCS(rotation);
}

MonoObject* GetGlobalRotation(MonoObject* go)
{
	TransformComponent* tr = GetComponentMono<TransformComponent*>(go);
	Quat rotation = Quat::identity;
	float3 p, s;
	tr->GetGlobalTransform().Decompose(p, rotation, s);
	return app->moduleMono->QuatToCS(rotation);
}

MonoObject* GetScale(MonoObject* go)
{
	TransformComponent* tr = GetComponentMono<TransformComponent*>(go);
	float3 scale = tr->GetLocalTransform().GetScale();
	return app->moduleMono->Float3ToCS(scale);
}

void SetPosition(MonoObject* go, MonoObject* position)
{
	if (TransformComponent* tr = GetComponentMono<TransformComponent*>(go))
	{
		tr->SetPosition(app->moduleMono->UnboxVector(position));
		tr->ForceUpdateTransform();
	}
}

void SetRotation(MonoObject* go, MonoObject* rotation)
{
	if (TransformComponent* tr = GetComponentMono<TransformComponent*>(go))
	{
		// Should update inspector rotation too?
		tr->SetRotation(app->moduleMono->UnboxQuat(rotation));
		tr->ForceUpdateTransform();
	}
}

void SetScale(MonoObject* go, MonoObject* scale)
{
	if (TransformComponent* tr = GetComponentMono<TransformComponent*>(go))
	{
		tr->SetScale(app->moduleMono->UnboxVector(scale));
		tr->ForceUpdateTransform();
	}
}

// Transform ========