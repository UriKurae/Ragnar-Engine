
#include "InputActionMenu.h"
#include "IconsFontAwesome5.h"
#include <string>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "Imgui/imgui_internal.h"

InputActionMenu::InputActionMenu() : Menu(true)
{
}

InputActionMenu::~InputActionMenu()
{
}

bool InputActionMenu::Start()
{
	bool ret = true;

	return ret;
}

bool InputActionMenu::Update(float dt)
{
	bool ret = true;
	ImGui::Begin(ICON_FA_KEYBOARD" Input Actions", &active);

	ImGui::Columns(3, nullptr);

	ImGui::BeginChild("Action Maps");
	ImGui::Separator();
	ImGui::Text("Action Maps");
	ImGui::SameLine();
	std::string text = ICON_FA_PLUS;
	auto posX = (ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(text.c_str()).x
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	if (posX > ImGui::GetCursorPosX())
		ImGui::SetCursorPosX(posX);
	if (ImGui::Button(text.c_str()))
	{

	}
	ImGui::Separator();
	ImGui::EndChild();

	ImGui::NextColumn();

	ImGui::BeginChild("Actions");
	ImGui::Separator();
	ImGui::Text("Actions");
	ImGui::SameLine();
	posX = (ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(text.c_str()).x
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	if (posX > ImGui::GetCursorPosX())
		ImGui::SetCursorPosX(posX);
	if (ImGui::Button(text.c_str()))
	{

	}
	ImGui::Separator();
	ImGui::EndChild();

	ImGui::NextColumn();
	ImGui::BeginChild("Properties");
	ImGui::Separator();
	ImGui::Text("Properties");
	ImGui::Separator();
	ImGui::EndChild();

	ImGui::End();
	return ret;
}

bool InputActionMenu::CleanUp()
{
	bool ret = true;

	return ret;
}
