#pragma once
#include "Module.h"

#include "SDL_scancode.h"
#include "SDL_mouse.h"

#include <vector>
#include <string>

#define MAX_MOUSE_BUTTONS 5

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(bool startEnabled = true);
	~ModuleInput();

	bool Init(JsonParsing& node) override;
	bool PreUpdate(float dt) override;
	bool CleanUp();

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) override;

	inline KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	inline KeyState GetMouseButton(int id) const
	{
		return mouseButtons[id];
	}

	inline int GetMouseX() const
	{
		return mouseX;
	}

	inline int GetMouseY() const
	{
		return mouseY;
	}

	inline int GetMouseZ() const
	{
		return mouseZ;
	}

	inline float GetMouseXMotion() const
	{
		return mouseXMotion;
	}

	inline float GetMouseYMotion() const
	{
		return mouseYMotion;
	}

	inline std::vector<std::string> GetInputList() const
	{
		return strings;
	}

private:
	KeyState* keyboard;
	KeyState mouseButtons[MAX_MOUSE_BUTTONS];
	int mouseX;
	int mouseY;
	int mouseZ;
	float mouseXMotion;
	float mouseYMotion;
	int mouseZMotion;

	std::vector<std::string> strings;

	bool repeated = true;
};