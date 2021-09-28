#include "ConsoleMenu.h"

ConsoleMenu::ConsoleMenu()
{
	buf.clear();
}

ConsoleMenu::~ConsoleMenu()
{
	buf.clear();
}

bool ConsoleMenu::Update()
{
	ImGui::Begin("Console");
	ImGui::TextUnformatted(buf.begin());
	if (scrollToBottom)
		ImGui::SetScrollHereY(1.0f);
	scrollToBottom = false;
	ImGui::End();

	return true;
}

void ConsoleMenu::AddLog(const char* fmt)
{
	va_list args;
	va_start(args, fmt);
	buf.appendfv(fmt, args);
	va_end(args);
	scrollToBottom = true;
}