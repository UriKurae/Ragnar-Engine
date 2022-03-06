
#include "Application.h"
#include "FileSystem.h"
#include "InputActionMenu.h"
#include "IconsFontAwesome5.h"
#include "Globals.h"
#include <string>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "Imgui/imgui_internal.h"

InputActionMenu::InputActionMenu() : Menu(false)
{
	actionMaps.push_back(new ActionMaps());
	currentMap = NULL;
	currentAction = NULL;
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
		actionMaps.push_back(new ActionMaps());
	}
	ImGui::Separator();
	for (size_t i = 0; i < actionMaps.size(); i++)
	{
		if (ImGui::TreeNodeEx(actionMaps[i]->GetName().c_str(), (actionMaps[currentMap] == actionMaps[i] ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf))
		{
			if (ImGui::IsItemClicked())
			{
				currentMap = i;
			}
			ImGui::TreePop();
		}
	}
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
		actionMaps[currentMap]->GetActions()->push_back(new Actions());
	}
	ImGui::Separator();
	for (size_t i = 0; i < actionMaps[currentMap]->GetActions()->size(); i++)
	{
		if (ImGui::TreeNodeEx(actionMaps[currentMap]->GetActions()->at(i)->GetName().c_str(), (actionMaps[currentMap]->GetActions()->at(currentAction) == actionMaps[currentMap]->GetActions()->at(i) ? ImGuiTreeNodeFlags_Selected : 0)))
		{
			if (ImGui::IsItemClicked())
			{
				currentAction = i;
			}
			ImGui::TreePop();
		}
	}
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
	JSON_Array* array = file.SetNewJsonArray(file.GetRootValue(), "Action Maps");
	for (int i = 0; i < actionMaps.size(); i++)
	{
		actionMaps[i]->OnSave(file, array);
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
	actionMaps.clear();

	JsonParsing sceneFile = JsonParsing();

	if (sceneFile.ParseFile(path) > 0)
	{
		JSON_Array* jsonArray = sceneFile.GetJsonArray(sceneFile.ValueToObject(sceneFile.GetRootValue()), "Action Maps");

		size_t size = sceneFile.GetJsonArrayCount(jsonArray);
		for (int i = 0; i < size; ++i)
		{
			JsonParsing go = sceneFile.GetJsonArrayValue(jsonArray, i);
			ActionMaps* aM = new ActionMaps();
			aM->OnLoad(go);
			actionMaps.push_back(aM);
		}
	}
	else
	{
		DEBUG_LOG("Input Asset couldn't be loaded");
	}
	return false;
}

Actions::Actions()
{
	name = "Move";
}

Actions::~Actions()
{
}

void Actions::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Name", name.c_str());

	node.SetValueToArray(array, file.GetRootValue());
}

void Actions::OnLoad(JsonParsing& node)
{
	name = node.GetJsonString("Name");
}

ActionMaps::ActionMaps()
{
	name = "Player";
	actions.push_back(new Actions());
}

ActionMaps::~ActionMaps()
{
}

void ActionMaps::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Name", name.c_str());

	JSON_Array* newArray = file.SetNewJsonArray(file.GetRootValue(), "Actions");
	for (int i = 0; i < actions.size(); i++)
	{
		actions[i]->OnSave(file, newArray);
	}
	node.SetValueToArray(array, file.GetRootValue());
}

void ActionMaps::OnLoad(JsonParsing& node)
{
	name = node.GetJsonString("Name");

	JSON_Array* jsonArray = node.GetJsonArray(node.ValueToObject(node.GetRootValue()), "Actions");

	size_t size = node.GetJsonArrayCount(jsonArray);
	for (int i = 0; i < size; ++i)
	{
		JsonParsing c = node.GetJsonArrayValue(jsonArray, i);
		Actions* a = new Actions();
		a->OnLoad(c);
		actions.push_back(a);
	}
}
