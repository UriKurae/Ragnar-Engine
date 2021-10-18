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

	virtual void OnEditor() {}

protected:
	ComponentType type;
	bool active;
	GameObject* owner;
};