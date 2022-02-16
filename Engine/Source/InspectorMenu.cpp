#include "InspectorMenu.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"

#include "GameObject.h"
#include "Resource.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_internal.h"

#include "Profiling.h"

InspectorMenu::InspectorMenu() : Menu(true)
{
	tags = { "Untagged", "Respawn", "Finish", "EditorOnly", "MainCamara", "Player", "GameController" };
	layers = { "Default", "TransparentFX", "Ignore Raycast", "Water", "UI", "Player" };
}

InspectorMenu::~InspectorMenu()
{
}

bool InspectorMenu::Update(float dt)
{
	ImGui::Begin("Inspector", &active);
	if (!app->scene->GetRoot()->GetChilds().empty())
	{
		// The inspector is empty if no object is selected
		GameObject* obj = app->editor->GetGO();
		if (obj != nullptr)
		{
			if (item == ItemType::NONE) 
			{
				DrawDefaultInspector(obj);
				obj->DrawEditor();
			}
			else {
				DrawEditLists();
			}

			// TODO: The line below is not necessary anymore, it's "HardCoded", does not function like unity's inspector
			//object.DrawEditor();
		}
		else if (app->editor->GetResource())
		{
			app->editor->GetResource()->DrawOnEditor();
		}
	}
	ImGui::End();

	return true;
}

void InspectorMenu::DrawDefaultInspector(GameObject* obj)
{
	ImGui::PushMultiItemsWidths(3, ImGui::GetWindowWidth());
	ImGui::Checkbox("##Active", &obj->active);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::Text("Name");
	ImGui::SameLine();
	ImGui::InputText("##Name", &obj->name[0], 30);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	if (ImGui::Checkbox("Static", &obj->staticObj)) {
		RecursiveSetStaticObjects(obj, obj->staticObj);
	}
	ImGui::PopItemWidth();

	// Draw tagList and layerList
	DrawList("Tag", &tags, obj->tag);
	ImGui::SameLine();
	DrawList("Layer", &layers, obj->layer);
	ImGui::Checkbox("Colliders", &obj->colliders);
	// Destroy object selected, pendingToDelete = true

	ImGui::Separator();
	for (int i = 0; i < obj->GetComponents().size(); ++i)
	{
		obj->GetComponents()[i]->OnEditor();
	}
	ImGui::NewLine();

	// Draw Add Component button
	//DrawAddComponent();
}

void InspectorMenu::DrawEditLists()
{
	if (ImGui::Button("Back"))
		item = ItemType::NONE;

	// System to determine which node starts open 
	ImGuiTreeNodeFlags flag = 0;
	if (item == ItemType::TAG) flag = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Tags", flag))
	{
		DrawListTagLayer("Tag", tags);
	}

	if (item == ItemType::LAYER) flag = ImGuiTreeNodeFlags_DefaultOpen;
	else flag = 0;
	if (ImGui::CollapsingHeader("Layers", flag))
	{
		DrawListTagLayer("Layer", layers);
	}
}

void InspectorMenu::RecursiveSetStaticObjects(GameObject* obj, bool ret)
{
	obj->staticObj = ret;
	for (int i = 0; i < obj->GetChilds().size(); i++)
	{
		RecursiveSetStaticObjects(obj->GetChilds()[i], ret);
	}
}

std::string InspectorMenu::DrawList(const char* label, std::vector<std::string>* list, std::string& item)
{
	std::string ret = "";
	ImGui::Text(label); ImGui::SameLine();

	//// Set the width of item
	ImGui::PushItemWidth(ImGui::GetWindowSize().x/2 - ImGui::CalcTextSize(label).x - 15);

	std::string listLabel = "##List";
	listLabel.append(label);

	// Draw the popUp tab of a list and the state of current item of in the list has a special style
	if (ImGui::BeginCombo(listLabel.c_str(), item.c_str()))
	{
		for (int i = 0; i < list->size(); i++)
		{
			bool isSelected = !item.compare(list->at(i));
			if (ImGui::Selectable(list->at(i).c_str(), isSelected))
				item = list->at(i);
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::Separator();
		AddItem(label);
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	return ret;
}

void InspectorMenu::AddItem(const char* label)
{
	std::string addItem = "Add";
	addItem.append(label);
	if (ImGui::Button(addItem.c_str()))
	{
		if (label == "Tag")
			item = ItemType::TAG;
		else if (label == "Layer")
			item = ItemType::LAYER;
	}
}

void InspectorMenu::DrawListTagLayer(const char* label, std::vector<std::string> list)
{
	for (int i = 0; i < list.size(); i++)
	{
		ImGui::Text("%s %d: ", label, i);	ImGui::SameLine();
		ImGui::Text(list.at(i).c_str());
	}
	ImGui::Separator();
	ImGui::Text("New %s: ", label);	ImGui::SameLine();

	ImGui::PushItemWidth(100);
	if (label == "Tag")
		ImGui::InputTextWithHint("  ", "Add new Tag", newTag, IM_ARRAYSIZE(newTag));
	else
		ImGui::InputTextWithHint(" ", "Add new Layer", newLayer, IM_ARRAYSIZE(newLayer));
	ImGui::PopItemWidth();

	ImGui::SameLine();
	ImGui::PushID(label);
	if (ImGui::Button("Add ICON_FA_PLUS"))
	{
		if (label == "Tag")
		{
			AddTag(newTag);
			newTag[0] = NULL;
		}
		else if (newLayer)
		{
			AddLayer(newLayer);
			newLayer[0] = NULL;
		}
	}
	ImGui::PopID();
}

void InspectorMenu::AddTag(std::string newTag)
{
	if (newTag != "")
		tags.push_back(newTag);
}

void InspectorMenu::AddLayer(std::string newLayer)
{
	if (newLayer != "")
		layers.push_back(newLayer);
}
