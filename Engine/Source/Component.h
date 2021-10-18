#pragma once

class GameObject;

enum class ComponentType
{
	NONE = -1,
	TRANSFORM,
	MESH_RENDERER,
	MATERIAL
};

class Component
{
public:
	Component() :active(true), type(ComponentType::NONE), owner(nullptr) {}
	virtual ~Component() {}

	virtual void Enable() {}
	virtual bool Update(float dt) { return true; }
	virtual void Disable() {}
	virtual void Draw() {}

	virtual void OnEditor() {}

	inline void SetOwner(GameObject* own) { owner = own; }

protected:
	ComponentType type;
	bool active;
	GameObject* owner;
};