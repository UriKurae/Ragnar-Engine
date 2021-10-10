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

	inline bool& GetActive() { return console; }

private:
	ImGuiTextBuffer buf;
	bool scrollToBottom;

	bool console;
	bool collapsed;
	ImVec2 size;
};