#include "ConsoleMenu.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#include "mmgr/mmgr.h"

ConsoleMenu::ConsoleMenu() : Menu(true)
{
	scrollToBottom = false;
	console = true;
	collapsed = false;
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