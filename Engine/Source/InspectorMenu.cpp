#include "InspectorMenu.h"
#include "Application.h"

#include "ModuleEditor.h"
#include "ModuleSceneManager.h"
#include "Scene.h"

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
	ImGui::Begin(ICON_FA_INFO_CIRCLE" Inspector", &active);
	if (!app->sceneManager->GetCurrentScene()->GetRoot()->GetChilds().empty())
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
			else 
			{
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
	// Destroy object selected, pendingToDelete = true

	if (obj->GetPrefabID() != 0)
	{
		ImGui::Text("Prefab ID: %d", obj->GetPrefabID());
		ImGui::Text("Prefab Path: %s", obj->prefabPath.c_str());
	}

	int comp_origin = -1, comp_destination = -1;
	ImGui::Separator();
	for (int i = 0; i < obj->GetComponents().size(); ++i)
	{
		ImGui::PushID(i);
		obj->GetComponents()[i]->OnEditor();
		ImGui::PopID();
	}
	ImGui::NewLine();

	/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

	//OPTIONAL TODO: Drag'n'Drop for component reorganitation. Code example below.

	/*{
		ImGui::BulletText("Drag and drop to re-order");
		static const char* names[6] = { "1. Adbul", "2. Alfonso", "3. Aline", "4. Amelie", "5. Anna", "6. Arthur" };
		int move_from = -1, move_to = -1;
		for (int n = 0; n < IM_ARRAYSIZE(names); n++)
		{
			ImGui::Selectable(names[n]);

			ImGuiDragDropFlags src_flags = 0;
			src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
			src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
			src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip
			if (ImGui::BeginDragDropSource(src_flags))
			{
				if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
					ImGui::Text("Moving \"%s\"", names[n]);
				ImGui::SetDragDropPayload("DND_DEMO_NAME", &n, sizeof(int));
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				ImGuiDragDropFlags target_flags = 0;
				target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
				target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_NAME", target_flags))
				{
					move_from = *(const int*)payload->Data;
					move_to = n;
				}
				ImGui::EndDragDropTarget();
			}
		}

		if (move_from != -1 && move_to != -1)
		{
			// Reorder items
			int copy_dst = (move_from < move_to) ? move_from : move_to + 1;
			int copy_src = (move_from < move_to) ? move_from + 1 : move_to;
			int copy_count = (move_from < move_to) ? move_to - move_from : move_from - move_to;
			const char* tmp = names[move_from];
			//printf("[%05d] move %d->%d (copy %d..%d to %d..%d)\n", ImGui::GetFrameCount(), move_from, move_to, copy_src, copy_src + copy_count - 1, copy_dst, copy_dst + copy_count - 1);
			memmove(&names[copy_dst], &names[copy_src], (size_t)copy_count * sizeof(const char*));
			names[move_to] = tmp;
			ImGui::SetDragDropPayload("DND_DEMO_NAME", &move_to, sizeof(int)); // Update payload immediately so on the next frame if we move the mouse to an earlier item our index payload will be correct. This is odd and showcase how the DnD api isn't best presented in this example.
		}
		ImGui::NewLine();
	}*/
}

void InspectorMenu::DrawEditLists()
{
	if (ImGui::Button(ICON_FA_BACKWARD" Back"))
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

void InspectorMenu::DrawListTagLayer(const char* label, std::vector<std::string>& list)
{
	for (int i = 0; i < list.size(); ++i)
	{
		ImGui::Text("%s %d: ", label, i);	
		ImGui::SameLine();
		ImGui::Text(list.at(i).c_str());
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - 15.0f);		
		(strcmp(label, "Layer") == 0)? ImGui::PushID(i) : ImGui::PushID(i+30); // 30 must be more than tagList.size
		if (ImGui::Button(ICON_FA_TRASH))
		{
			for (std::vector<std::string>::iterator it = list.begin(); it != list.end(); ++it)
			{
				if ((*it) == list.at(i))
				{
					list.erase(it);
					break;
				}
			}
		}
		ImGui::PopID();
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
	if (ImGui::Button(ICON_FA_PLUS" Add"))
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
