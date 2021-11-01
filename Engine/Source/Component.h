#pragma once

#include "Imgui/imgui.h"

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
	Component() : active(true), type(ComponentType::NONE), owner(nullptr), collapsed(false) {}
	virtual ~Component() {}

	virtual void Enable() {}
	virtual bool Update(float dt) { return true; }
	virtual void Disable() {}
	virtual void Draw() {}

	virtual void OnEditor() {}

	inline void SetOwner(GameObject* own) { owner = own; }

	void Checkbox(Component* component, const char* name, bool& act)
	{
		ImGui::PushID((void*)component);

		ImGui::Checkbox(name, &act);

		ImGui::PopID();
	}

	inline const ComponentType& GetType() const { return type; }
	inline const bool& GetActive() const { return active; }

public:
	ComponentType type;
	bool active;
	GameObject* owner;

	bool collapsed;
};