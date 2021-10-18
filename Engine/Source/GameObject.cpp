#include "GameObject.h"

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

void GameObject::Draw()
{
	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->Draw();
	}
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

void GameObject::AddComponent(Component* component)
{
	component->SetOwner(this);
	components.emplace_back(component);
}

void GameObject::AddChild(GameObject* object)
{
	children.emplace_back(object);
}