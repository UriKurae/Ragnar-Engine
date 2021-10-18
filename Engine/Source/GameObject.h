#pragma once

#include <string>
#include <vector>
#include "Component.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	bool Update(float dt);

	Component* CreateComponent(ComponentType type);

	inline const char* GetName() const { return name.c_str(); }
	inline bool GetActive() const { return active; }

	template<typename T>
	T* GetComponent();
private:
	std::string name;
	bool active;
	std::vector<Component*> components;

	GameObject* parent;
	std::vector<GameObject*> children;
};

template<typename T>
inline T* GameObject::GetComponent()
{
	T* component = nullptr;

	for (int i = 0; i < components.size(); ++i)
	{
		component = dynamic_cast<T*>(i);
		if (component != nullptr)
			return component;
	}

	return component;
}