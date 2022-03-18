#pragma once
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleSceneManager.h"

#include "ResourceManager.h"

#include "ButtonComponent.h"
#include "MaterialComponent.h"
#include "Texture.h"
#include "Scene.h"
#include "TransformBindings.h"

#include <queue>

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
		tr->UpdateTransform();
	}
}

void SetRotation(MonoObject* go, MonoObject* rotation)
{
	if (TransformComponent* tr = GetComponentMono<TransformComponent*>(go))
	{
		// Should update inspector rotation too?
		tr->SetRotation(app->moduleMono->UnboxQuat(rotation));
		tr->UpdateTransform();
	}
}

void SetScale(MonoObject* go, MonoObject* scale)
{
	if (TransformComponent* tr = GetComponentMono<TransformComponent*>(go))
	{
		tr->SetScale(app->moduleMono->UnboxVector(scale));
		tr->UpdateTransform();
	}
}
// Transform ========


MonoString* GetTexturePath(MonoObject* go)
{
	MaterialComponent* matComp = GetComponentMono<MaterialComponent*>(go);
	std::string p = matComp->GetTexture()->GetAssetsPath();
	return mono_string_new(app->moduleMono->domain, p.c_str());
}

void SetTexturePath(MonoObject* go, MonoObject* texturePath)
{
	MaterialComponent* matComp = GetComponentMono<MaterialComponent*>(go);
	char* path = mono_string_to_utf8(mono_object_to_string(texturePath, 0));
	std::string p = path;

	std::shared_ptr<Texture> newTexture = std::static_pointer_cast<Texture>(ResourceManager::GetInstance()->LoadResource(p));
	matComp->SetTexture(newTexture);
}


// GameObject =======================
MonoObject* InstantiateGameObject(MonoObject* name, MonoObject* position, MonoObject* rotation)
{
	GameObject* go = app->sceneManager->GetCurrentScene()->CreateGameObject(nullptr);
	char* goName = mono_string_to_utf8(mono_object_to_string(name, 0));
	go->SetName(goName);
	mono_free(goName);

	float3 p = app->moduleMono->UnboxVector(position);
	TransformComponent* tr = go->GetComponent<TransformComponent>();
	tr->SetPosition(p);

	Quat r = app->moduleMono->UnboxQuat(rotation);
	tr->SetRotation(r);
	return app->moduleMono->GoToCSGO(go);
}

void Instantiate3DObject(MonoObject* name, int primitiveType, MonoObject* position, MonoObject* rotation)
{
	Object3D t = static_cast<Object3D>(primitiveType);
	GameObject* go = app->sceneManager->GetCurrentScene()->Create3DObject(t, nullptr);
	char* goName = mono_string_to_utf8(mono_object_to_string(name, 0));
	go->SetName(goName);
	mono_free(goName);

	float3 p = app->moduleMono->UnboxVector(position);
	TransformComponent* tr = go->GetComponent<TransformComponent>();
	tr->SetPosition(p);

	Quat r = app->moduleMono->UnboxQuat(rotation);
	tr->SetRotation(r);
}

MonoObject* Instantiate3DGameObject(MonoObject* name, int primitiveType, MonoObject* position)
{
	Object3D t = static_cast<Object3D>(primitiveType);
	GameObject* go = app->sceneManager->GetCurrentScene()->Create3DObject(t, nullptr);
	char* goName = mono_string_to_utf8(mono_object_to_string(name, 0));
	go->SetName(goName);
	mono_free(goName);

	float3 p = app->moduleMono->UnboxVector(position);
	TransformComponent* tr = go->GetComponent<TransformComponent>();
	tr->SetPosition(p);
	tr->UpdateTransform();

	return app->moduleMono->GoToCSGO(go);
}

MonoObject* AddComponentMono(MonoObject* go, int componentType)
{
	char* goName = mono_string_to_utf8(mono_object_to_string(go, 0));

	GameObject* owner = app->moduleMono->GameObjectFromCSGO(go);
	Component* comp = owner->CreateComponent(static_cast<ComponentType>(componentType));

	return app->moduleMono->ComponentToCS(comp);
}

MonoObject* FindGameObjectWithName(MonoObject* name)
{
	char* goName = mono_string_to_utf8(mono_object_to_string(name, 0));

	std::queue<GameObject*> q;
	for (auto& go : app->scene->GetGameObjectsList())
		q.push(go);

	while (!q.empty())
	{
		GameObject* curr = q.front();
		q.pop();

		if (curr->GetName() == std::string(goName))
		{
			mono_free(goName);
			return app->moduleMono->GoToCSGO(curr);
		}

		for (auto& child : curr->GetChilds())
			q.push(child);
	}

	mono_free(goName);
	return nullptr;
}

MonoArray* FindGameObjectsWithTag(MonoObject* tag)
{
	char* tagName = mono_string_to_utf8(mono_object_to_string(tag, 0));
	
	std::vector<MonoObject*> objects;
	for (auto& go : app->scene->GetGameObjectsList())
	{
		if (go->tag == tagName)
			objects.push_back(app->moduleMono->GoToCSGO(go));
	}
	
	MonoClass* goClass = mono_class_from_name(app->moduleMono->image, SCRIPTS_NAMESPACE, "GameObject");
	MonoArray* ret = mono_array_new(app->moduleMono->domain, goClass, objects.size());
	
	for (int i = 0; i < objects.size(); ++i)
		mono_array_set(ret, MonoObject*, i, objects[i]);

	mono_free(tagName);
	return ret;
}

MonoArray* GetGameObjectChilds(MonoObject* go)
{
	GameObject* gameObject = app->moduleMono->GameObjectFromCSGO(go);

	std::vector<MonoObject*> objects;
	for (auto& child : gameObject->GetChilds())
		objects.push_back(app->moduleMono->GoToCSGO(child));

	MonoClass* goClass = mono_class_from_name(app->moduleMono->image, SCRIPTS_NAMESPACE, "GameObject");
	MonoArray* ret = mono_array_new(app->moduleMono->domain, goClass, objects.size());

	for (int i = 0; i < objects.size(); ++i)
		mono_array_set(ret, MonoObject*, i, objects[i]);

	return ret;
}

MonoString* GetGameObjectTagMono(MonoObject* go)
{
	GameObject* gameObject = app->moduleMono->GameObjectFromCSGO(go);
	return mono_string_new(app->moduleMono->domain, gameObject->tag.c_str());
}

void SetGameObjectTagMono(MonoObject* go, MonoString* newTag)
{
	GameObject* gameObject = app->moduleMono->GameObjectFromCSGO(go);
	char* tagName = mono_string_to_utf8(newTag);
	gameObject->tag = tagName;
}

MonoString* GetGameObjectName(MonoObject* go)
{
	GameObject* gameObject = app->moduleMono->GameObjectFromCSGO(go);
	return mono_string_new(app->moduleMono->domain, gameObject->name.c_str());
}

void SetGameObjectName(MonoObject* go, MonoString* newName)
{
	GameObject* gameObject = app->moduleMono->GameObjectFromCSGO(go);
	char* name = mono_string_to_utf8(newName);
	gameObject->name = name;
}

MonoBoolean GetGameObjectIsActive(MonoObject* go)
{
	GameObject* gameObject = app->moduleMono->GameObjectFromCSGO(go);
	return gameObject->active;
}

void SetGameObjectIsActive(MonoObject* go, MonoBoolean value)
{
	GameObject* gameObject = app->moduleMono->GameObjectFromCSGO(go);
	gameObject->active = value;
}
// GameObject =======================

// UI ===============================
MonoString* GetButtonText(MonoObject* go)
{
	ButtonComponent* button = GetComponentMono<ButtonComponent*>(go);
	return mono_string_new(app->moduleMono->domain, button->GetText());
}

void SetButtonText(MonoObject* go, MonoString* text)
{
	ButtonComponent* button = GetComponentMono<ButtonComponent*>(go);
	//button->SetText(mono_string_to_utf8(text));
}
// UI ===============================


float GetGameTimeStep()
{
	return app->sceneManager->GetCurrentScene()->GetGameDeltaTime();
}