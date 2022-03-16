
#include "Application.h"
#include "FileSystem.h"
#include "InputActionComponent.h"
#include "InputActionMenu.h"
#include "Globals.h"

#include "IconsFontAwesome5.h"
#include "Profiling.h"

InputActionComponent::InputActionComponent(GameObject* own)
{
	owner = own;
	type = ComponentType::INPUT_ACTION;
	assetWindowActive = false;
}

InputActionComponent::~InputActionComponent()
{
}

void InputActionComponent::OnEditor()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader(ICON_FA_GAMEPAD" Player Input"))
	{
		ImGui::Text("Actions:");
		ImGui::SameLine();
		std::string text = currentAssetName + (std::string)ICON_FA_EDIT;
		auto posX = (ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(text.c_str()).x
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		if (posX > ImGui::GetCursorPosX())
			ImGui::SetCursorPosX(posX);
		ImGui::Text(currentAssetName.c_str());
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_EDIT))
		{
			assetWindowActive = true;
			//inputAssetsList.clear();
			LoadAllInputAssets("Assets");
		}

		if (currentActionMaps.size() > 0)
		{
			if (ImGui::TreeNodeEx("Events", ImGuiTreeNodeFlags_DefaultOpen))
			{
				for (size_t i = 0; i < currentActionMaps.size(); i++)
				{
					ImGui::PushID(i);
					if (ImGui::TreeNodeEx(currentActionMaps[i]->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGui::TreePop();
					}
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
		}
	}
	ImGui::PopID();

	if (assetWindowActive)
	{
		ImGui::Begin("Input Action Assets", &assetWindowActive, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 size = ImGui::GetWindowSize();
		ImVec2 mouse = ImGui::GetIO().MousePos;
		if (!(mouse.x < winPos.x + size.x && mouse.x > winPos.x &&
			mouse.y < winPos.y + size.y && mouse.y > winPos.y))
		{
			if (ImGui::GetIO().MouseClicked[0]) assetWindowActive = false;
		}

		if (ImGui::BeginTable("table3", 3))
		{
			for (int item = 0; item < inputAssetsList.size(); item++)
			{
				ImGui::TableNextColumn();
				std::string assetName = inputAssetsList[item];
				app->fs->GetFilenameWithoutExtension(assetName);
				if (ImGui::Selectable(assetName.c_str()))
				{
					assetWindowActive = false;
					LoadInputAsset(inputAssetsList[item].c_str());
				}
			}
			ImGui::EndTable();
		}

		ImGui::End();
	}
}

bool InputActionComponent::Update(float dt)
{
	return false;
}

bool InputActionComponent::OnLoad(JsonParsing& node)
{
	return false;
}

bool InputActionComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	return false;
}

bool InputActionComponent::LoadInputAsset(const char* path)
{
	currentActionMaps.clear();

	JsonParsing sceneFile = JsonParsing();

	if (sceneFile.ParseFile(path) > 0)
	{
		JSON_Array* jsonArray = sceneFile.GetJsonArray(sceneFile.ValueToObject(sceneFile.GetRootValue()), "Action Maps");

		size_t size = sceneFile.GetJsonArrayCount(jsonArray);
		for (int i = 0; i < size; ++i)
		{
			JsonParsing go = sceneFile.GetJsonArrayValue(jsonArray, i);
			std::shared_ptr<ActionMaps> aM(new ActionMaps());
			aM->OnLoad(go);
			currentActionMaps.push_back(aM);
		}
		currentAssetName = path;
	}
	else
	{
		DEBUG_LOG("Input Asset couldn't be loaded");
	}
	return false;
}

void InputActionComponent::LoadAllInputAssets(const char* folder)
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;
	std::string path = folder + std::string("/");
	app->fs->DiscoverFilesAndDirs(path.c_str(), files, dirs);
	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
	{
		if ((*it).find(".inputaction") != std::string::npos)
		{
			inputAssetsList.push_back(*it);
		}
	}
	for (std::vector<std::string>::iterator it = dirs.begin(); it != dirs.end(); ++it)
	{
		LoadInputAsset((*it).c_str());
	}
}
