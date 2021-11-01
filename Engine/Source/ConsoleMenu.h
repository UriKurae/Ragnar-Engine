#pragma once

#include "Menu.h"

#include "Imgui/imgui.h"

class ConsoleMenu : public Menu
{
public:
	ConsoleMenu();

	~ConsoleMenu();

	bool Update(float dt) override;

	void AddLog(const char* fmt);

	void ClearConsole();

	inline bool& GetActive() { return active; }

private:
	ImGuiTextBuffer buf;

	ImVec2 size;
};