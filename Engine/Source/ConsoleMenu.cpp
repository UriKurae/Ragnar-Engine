#include "ConsoleMenu.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#include "Profiling.h"

ConsoleMenu::ConsoleMenu() : Menu(true)
{
}

ConsoleMenu::~ConsoleMenu()
{
	buf.clear();
}

bool ConsoleMenu::Update(float dt)
{
	bool ret = false;

	//if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KeyState::KEY_REPEAT && 
	//	app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	//{
	//	collapsed = !collapsed;
	//	ImGui::SetNextWindowCollapsed(collapsed);
	//}

	ImGui::Begin("Console", &active);
	if (ImGui::Button("Clear console"))
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