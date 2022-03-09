
#include "Application.h"
#include "ModuleInput.h"
#include "FileSystem.h"
#include "InputActionMenu.h"
#include "IconsFontAwesome5.h"
#include "Globals.h"
#include <string>

#include "SDL.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "Imgui/imgui_internal.h"

InputActionMenu::InputActionMenu() : Menu(false)
{
	actionMaps.push_back(new ActionMaps());
	currentMap = NULL;
	currentAction = NULL;
	currentBinding = NULL;
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
	
	ImGui::Begin(ICON_FA_KEYBOARD" Input Actions", &active, ImGuiWindowFlags_MenuBar);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15.0f, 0.0f));
	if (ImGui::BeginMenuBar())
	{
		ImGui::Text("Asset name: %s", currentInputAssetName.c_str());

		if (ImGui::Button("Save Asset"))
		{
			SaveInputActionFile(currentInputAssetPath.c_str());
		}

		ImGui::EndMenuBar();
	}
	ImGui::PopStyleVar();

	ImGui::Columns(3, nullptr);

	ImGui::BeginChild("Action Maps");
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
		bool open = ImGui::TreeNodeEx(actionMaps[i]->GetName().c_str(),
			(actionMaps[currentMap] == actionMaps[i] ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_FramePadding);

		if (ImGui::IsItemClicked())
		{
			currentMap = i;
		}

		if (open)
		{
			ImGui::TreePop();
		}
	}
	ImGui::EndChild();

	ImGui::NextColumn();

	ImGui::BeginChild("Actions");
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
		ImGui::PushID(i);
		bool open = ImGui::TreeNodeEx(actionMaps[currentMap]->GetActions()->at(i)->GetName().c_str(),
			ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen | (actionMaps[currentMap]->GetActions()->at(currentAction) == actionMaps[currentMap]->GetActions()->at(i) ? ImGuiTreeNodeFlags_Selected : 0));
		
		if (ImGui::IsItemClicked())
		{
			currentAction = i;
		}

		if (open)
		{
			for (size_t j = 0; j < actionMaps[currentMap]->GetActions()->at(i)->GetBindings().size(); j++)
			{
				ImGui::TreeNodeEx(SDL_GetScancodeName((SDL_Scancode)actionMaps[currentMap]->GetActions()->at(i)->GetBindings()[j]),
					(actionMaps[currentMap]->GetActions()->at(i)->GetBindings()[currentBinding] == actionMaps[currentMap]->GetActions()->at(i)->GetBindings()[j] ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_FramePadding);
				
				if (ImGui::IsItemClicked())
				{
					currentBinding = j;
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::PopID();
	}
	ImGui::EndChild();

	ImGui::NextColumn();

	ImGui::BeginChild("Properties");
	ImGui::Text("Properties");
	ImGui::Separator();
	ImGui::Text("Name");
	//ImGui::SameLine();
	//ImGui::InputText("##Name", &current, 30);
	ImGui::Text("Path: ");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##combo", currentBindingItem))
	{
		for (int n = 0; n < IM_ARRAYSIZE(app->input->keyNameList); n++)
		{
			if ((app->input->keyNameList[n] != NULL) && (app->input->keyNameList[n][0] != '\0'))
			{
				bool is_selected = (currentBindingItem == app->input->keyNameList[n]);
				if (ImGui::Selectable(app->input->keyNameList[n], is_selected))
				{
					currentBindingItem = app->input->keyNameList[n];
					//actionMaps[currentMap]->GetActions()->at(currentAction)->AddBinding(n);
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
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
		currentInputAssetName = path;
		app->fs->GetFilenameWithoutExtension(currentInputAssetName);
		currentInputAssetPath = path;
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

	JSON_Array* newArray = file.SetNewJsonArray(file.GetRootValue(), "Bindings");
	for (int i = 0; i < bindings.size(); i++)
	{
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Path", bindings[i]);
	}

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
	actions.clear();
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
