#pragma once

#include <string>
#include <vector>
#include "Component.h"
#include "TransformComponent.h"
#include "MeshComponent.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	bool Update(float dt);
	void Draw();
	void DrawEditor();

	Component* CreateComponent(ComponentType type);
	void AddComponent(Component* component);
	void AddChild(GameObject* object);

	inline void SetParent(GameObject* object) { parent = object; }
	inline void SetName(const char* n) { name = n; }

	char* GetNameBuffer();
	inline const char* GetName() const { return name.c_str(); }
	inline const GameObject* GetParent() const { return parent; }
	inline bool GetActive() const { return active; }
	inline std::vector<GameObject*>& GetChilds() { return children; }

	void MoveChildrenUp(GameObject *child);
	void MoveChildrenDown(GameObject *child);

	template<typename T>
	T* GetComponent();

	template<typename T>
	std::vector<T*> GetAllComponent();

	template<typename T>
	T* CreateComponent();
private:
	std::string name;
	bool active;
	std::vector<Component*> components;

	GameObject* parent;
	std::vector<GameObject*> children;

	bool newComponent;
};

template<typename T>
inline T* GameObject::GetComponent()
{
	T* component = nullptr;
	
	for (std::vector<Component*>::iterator i = components.begin(); i < components.end(); ++i)
	{
		component = dynamic_cast<T*>(*i);
		if (component != nullptr)
			return component;
	}

	return component;
}

template<typename T>
inline std::vector<T*> GameObject::GetAllComponent()
{
	std::vector<T*> component;

	for (std::vector<Component*>::iterator i = components.begin(); i < components.end(); ++i)
	{
		T* c= nullptr;
		c = dynamic_cast<T*>(*i);
		if (c != nullptr)
			component.push_back(c);
	}

	return component;
}