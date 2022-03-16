#pragma once

#include "Imgui/imgui.h"
#include "JsonParsing.h"
#include "IconsFontAwesome5.h"

class GameObject;
class CameraComponent;
class ButtonComponent;
class ComponentTransform2D;
class SliderComponent;
class ImageComponent;
class CheckboxComponent;

enum class ComponentType
{
	NONE = -1,
	TRANSFORM,
	MESH_RENDERER,
	MATERIAL,
	CAMERA,
	SCRIPT,
	AUDIO_SOURCE,
	AUDIO_LISTENER,
	AUDIO_REVERB_ZONE,
	INPUT_ACTION,
	LIGHT,
	ANIMATION,
	BONE,
	RIGID_BODY,
	UI_BUTTON,
	UI_IMAGE,
	UI_CHECKBOX,
	UI_SLIDER,
	UI_INPUTBOX,
	UI_CANVAS,
	TRANFORM2D,
	NAVAGENT,
};

enum class State
{
	DISABLED,
	NORMAL,
	FOCUSED,
	PRESSED,
	SELECTED
};

class Component
{
public:
	Component() : active(true), type(ComponentType::NONE), owner(nullptr), collapsed(false) {}
	virtual ~Component() {}

	virtual void Enable() {}
	virtual bool Update(float dt) { return true; }
	virtual void Disable() {}
	virtual void Draw(CameraComponent* gameCam = nullptr) {}
	virtual void DrawOutline() {}

	virtual void OnEditor() {}

	inline void SetOwner(GameObject* own) { owner = own; }

	void Checkbox(Component* component, const char* name, bool& act)
	{
		ImGui::PushID((void*)component);

		ImGui::Checkbox(name, &act);

		ImGui::PopID();
	}

	void ComponentOptions(Component* component);
	

	inline const ComponentType& GetType() const { return type; }
	inline const bool& GetActive() const { return active; }

	virtual bool OnLoad(JsonParsing& node) { return true; }
	virtual bool OnSave(JsonParsing& node, JSON_Array* array) { return true; }

public:
	ComponentType type;
	bool active;
	GameObject* owner;

	bool collapsed;
};