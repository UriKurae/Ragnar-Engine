
#include "Application.h"
#include "FileSystem.h"
#include "InputActionMenu.h"
#include "IconsFontAwesome5.h"
#include "Globals.h"
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

bool InputActionMenu::SaveInputActionFile(const char* path)
{
	DEBUG_LOG("Saving Input Action: %s", path);

	std::string fileName = path;
	app->fs->GetFilenameWithoutExtension(fileName);

	JsonParsing file;

	file = file.SetChild(file.GetRootValue(), "Input Action");
	JSON_Array* array = file.SetNewJsonArray(file.GetRootValue(), "");
	for (int i = 0; i < actionMaps.size(); i++)
	{
		actionMaps[i].OnSave(file, array);
	}

	uint size = file.SaveFile(path);

	if (size > 0)
		DEBUG_LOG("Input Action saved succesfully");
	else
		DEBUG_LOG("Input Action couldn't be saved");

	return true;
}

bool InputActionMenu::LoadInputActionFile(const char* path)
{
	return false;
}

Actions::Actions()
{
}

Actions::~Actions()
{
}

void Actions::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Name", name.c_str());
}

ActionMaps::ActionMaps()
{
}

ActionMaps::~ActionMaps()
{
}

void ActionMaps::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Name", name.c_str());
	for (int i = 0; i < actions.size(); i++)
	{
		actions[i].OnSave(node, array);
	}
}
