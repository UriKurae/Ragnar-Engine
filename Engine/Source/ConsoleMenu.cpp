#include "ConsoleMenu.h"
#include "Application.h"

#include "Profiling.h"

ConsoleMenu::ConsoleMenu() : Menu(false, "Console")
{
}

ConsoleMenu::~ConsoleMenu()
{
	buf.clear();
}

bool ConsoleMenu::Update(float dt)
{
	RG_PROFILING_FUNCTION("Console Menu Update");

	bool ret = false;

	ImGui::Begin(ICON_FA_WINDOW_MAXIMIZE" Console", &active);
	if (ImGui::Button(ICON_FA_POO" Clear console"))
	{
		ClearConsole();
	}
	ImGui::Separator();
	ImGui::BeginChild("LOG");
	ImGui::TextUnformatted(buf.begin());
	ImGui::EndChild();
	ImGui::End();

	return true;
}

void ConsoleMenu::AddLog(const char* fmt)
{
	va_list args;
	va_start(args, fmt);
	buf.appendfv(fmt, args);
	va_end(args);
}

void ConsoleMenu::ClearConsole()
{
	buf.clear();
}