#pragma once
#include "Module.h"
#include "SDL.h"

#include <vector>
#include <array>
#include <string>

#define MAX_MOUSE_BUTTONS 5

enum class KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

//Adapted from SDL_GameControllerAxis
enum class Axis
{
	LeftStickHorizontal,
	LeftStickVertical,
	RightStickHorizontal,
	RightStickVertical,
	LeftTrigger,
	RightTrigger,
	Count
};


//Adapted from SDL_GameControllerButton
enum class Button
{
	A,
	B,
	X,
	Y,
	Back,
	Guide,
	Start,
	LS,
	RS,
	LB,
	RB,
	DPAD_Up,
	DPAD_Down,
	DPAD_Left,
	DPAD_Right,
	Count
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(bool startEnabled = true);
	~ModuleInput();

	bool Init(JsonParsing& node) override;
	bool PreUpdate(float dt) override;
	bool CleanUp();

	bool IsJoystickAvailable(int joystickId);
	bool ControllerUpdate();

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) override;

	void AddController(int id);
	void RemoveController(int id);

	void FillScancodeNameList();
	inline std::vector<std::string> GetScancodeNameList() { return keyNameList; }

	inline KeyState GetKey(int id) const { return keyboard[id]; }
	inline KeyState GetMouseButton(int id) const { return mouseButtons[id]; }
	inline int GetMouseX() const { return mouseX; }
	inline int GetMouseY() const { return mouseY; }
	inline int GetMouseZ() const { return mouseZ; }
	inline int GetMouseXMotion() const { return mouseXMotion; }
	inline int GetMouseYMotion() const { return mouseYMotion; }

	bool GetButton(int joystickId, Button button);
	bool GetButtonDown(int joystickId, Button button);
	bool GetButtonUp(int joystickId, Button button);
	bool GetAxis(int joystickId, Axis axis);

	inline std::vector<std::string> GetInputList() const { return strings; }

private:
	KeyState* keyboard;
	KeyState mouseButtons[MAX_MOUSE_BUTTONS];
	int mouseX;
	int mouseY;
	int mouseZ;
	int mouseXMotion;
	int mouseYMotion;
	int mouseZMotion;
	
	SDL_GameController* pad;
	SDL_Joystick* joy;
	int joyID;
	std::array<bool, (int)Button::Count> buttons;
	std::array<bool, (int)Button::Count> lastButtons;
	std::array<float, (int)Axis::Count> axes;
	std::array<float, (int)Axis::Count> lastAxis;
	float deadzone;

	std::vector<std::string> strings;
	std::vector<std::string> keyNameList;

	bool repeated = true;
};