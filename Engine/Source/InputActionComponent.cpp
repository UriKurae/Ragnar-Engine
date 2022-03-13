
#include "InputActionComponent.h"
#include "InputActionMenu.h"
#include "Globals.h"

#include "IconsFontAwesome5.h"

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
			LoadInputAsset("Assets/InputAction.inputaction");
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
		ImGui::Begin("Input Action Assets", &assetWindowActive);
		


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
			ActionMaps* aM = new ActionMaps();
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
