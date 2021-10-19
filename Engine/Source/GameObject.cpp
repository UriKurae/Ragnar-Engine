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

void GameObject::MoveChildrenUp(GameObject* child)
{
	if (child == children[0]) return;

	for (int i = 0; i < children.size(); ++i)
	{
		if (children[i] == child)
		{
			GameObject* aux = child;

			children[i] = children[i - 1];
			children[i - 1] = aux;
		}
	}
}

void GameObject::MoveChildrenDown(GameObject* child)
{
	if (child == children[children.size()-1]) return;

	for (int i = children.size()-1; i >= 0; --i)
	{
		if (children[i] == child)
		{
			GameObject* aux = child;

			children[i] = children[i + 1];
			children[i + 1] = aux;
		}
	}
}
