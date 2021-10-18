#include "GameObject.h"

#include "TransformComponent.h"

GameObject::GameObject() : active(true), parent(nullptr)
{
}

GameObject::~GameObject()
{
}

bool GameObject::Update(float dt)
{
	return true;
}

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* component = nullptr;

	switch (type)
	{
	case ComponentType::TRANSFORM:
		component = new TransformComponent();
		break;
	case ComponentType::MESH_RENDERER:

		break;
	case ComponentType::MATERIAL:
		break;
	}

	return component;
}