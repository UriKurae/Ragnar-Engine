#pragma once

#include "Imgui/imgui.h"

class ConsoleMenu
{
public:
	ConsoleMenu();

	~ConsoleMenu();

	bool Update();

	void AddLog(const char* fmt);

private:
	ImGuiTextBuffer buf;
	bool scrollToBottom;
};