#include "HierarchyMenu.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleEditor.h"
#include "ModuleSceneManager.h"
#include "Scene.h"
#include "FileSystem.h"

#include "PrefabManager.h"

#include "Profiling.h"

HierarchyMenu::HierarchyMenu() : Menu(true, "Hierarchy")
{
	gameObjectOptions = false;
	confirmPanel = false;
}

HierarchyMenu::~HierarchyMenu()
{
}

bool HierarchyMenu::Update(float dt)
{
	RG_PROFILING_FUNCTION("Hierarchy Menu Update");

	ImGui::Begin(ICON_FA_SITEMAP" Hierarchy", &active, ImGuiWindowFlags_NoCollapse);

	GameObject* root = app->sceneManager->GetCurrentScene()->GetRoot();
	GameObject* selected = app->editor->GetGO();
	GameObject* selectedParent = app->editor->GetSelectedParent();
	ImGuiTreeNodeFlags flags = SetFlags(root) | ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::TreeNodeEx(root, flags, root->GetName()))
	{
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* go = ImGui::AcceptDragDropPayload("HierarchyItem");
			if (go)
			{
				uint goUuid = *(const uint*)(go->Data);
				app->sceneManager->GetCurrentScene()->ReparentGameObjects(goUuid, root);
			}
			ImGui::EndDragDropTarget();
		}
		ShowChildren(root);
		ImGui::TreePop();
	}

	if (!gameObjectOptions && ImGui::IsWindowHovered() && ImGui::IsWindowFocused() && (ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1]))
	{
		app->editor->SetGO(nullptr);
		app->editor->SetSelectedParent(nullptr);
	}

	if (gameObjectOptions)
	{
		ImGui::OpenPopup(ICON_FA_CUBE" GameObject");

		if (ImGui::BeginPopup(ICON_FA_CUBE" GameObject"))
		{
			if (ImGui::Button(ICON_FA_ARROW_UP" Move Up", ImVec2(100.0f, 30.0f)))
			{
				if (selectedParent != nullptr)
				{
					selectedParent->MoveChildrenUp(selected);
				}
				else
				{
					app->sceneManager->GetCurrentScene()->MoveGameObjectUp(selected);
				}
				gameObjectOptions = false;
			}
			else if (ImGui::Button(ICON_FA_ARROW_DOWN" Move Down", ImVec2(100.0f, 30.0f)))
			{
				if (selectedParent != nullptr)
				{
					selectedParent->MoveChildrenDown(selected);
				}
				else
				{
					app->sceneManager->GetCurrentScene()->MoveGameObjectDown(selected);
				}
				gameObjectOptions = false;
			}
			else if (ImGui::Button(ICON_FA_MINUS" Delete", ImVec2(100.0f, 30.0f)))
			{
				
				if (selected && selected->GetComponent<CameraComponent>() == nullptr)
				{
					for (std::vector<GameObject*>::iterator i = selectedParent->GetChilds().begin(); i != selectedParent->GetChilds().end(); ++i)
					{
						if (selected == (*i))
						{
							selectedParent->GetChilds().erase(i);
							RELEASE(selected);

							break;
						}
					}
				}
				app->editor->SetGO(nullptr);
				gameObjectOptions = false;
			}
			else if (ImGui::Button(ICON_FA_BOX_OPEN" Prefab", ImVec2(100.0f, 30.0f)))
			{
				if (app->fs->Exists(selected->prefabPath.c_str()) != 0)
				{
					confirmPanel = true;
					gameObjectOptions = false;
				}
				else
				{
					PrefabManager::GetInstance()->SavePrefab(selected, 1);
					gameObjectOptions = false;
				}
			}
			else if (!ImGui::IsAnyItemHovered() && ((ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1])))
			{
				gameObjectOptions = false;
			}

			if (selected != nullptr && selected->prefabPath != "None")
			{
				if (ImGui::Button(ICON_FA_BOX_OPEN" UnPrefab", ImVec2(100.0f, 30.0f)))
				{
					selected->prefabPath = "None";
					selected->UnPrefab();

					for (std::vector<GameObject*>::iterator it = selected->GetChilds().begin(); it != selected->GetChilds().end(); ++it)
					{
						(*it)->prefabPath = "None";
					}

					gameObjectOptions = false;
				}
			}

			ImGui::EndPopup();
		}
	}

	if (confirmPanel)
	{
		ImGui::OpenPopup("Confirm");

		if (ImGui::BeginPopup("Confirm"))
		{
			ImGui::Text("There's an existing prefab");
			ImGui::Text("with the same name: ");

			if (ImGui::Button("Overwrite", ImVec2(150.0f, 30.0f)))
			{
				PrefabManager::GetInstance()->SavePrefab(selected, 2);
				confirmPanel = false;
			}
			else if (ImGui::Button("Create new prefab", ImVec2(150.0f, 30.0f)))
			{
				PrefabManager::GetInstance()->SavePrefab(selected, 3);
				confirmPanel = false;
			}
			else if (ImGui::Button("Cancel", ImVec2(150.0f, 30.0f)))
			{
				confirmPanel = false;
			}
			else if (!ImGui::IsAnyItemHovered() && ((ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1])))
			{
				confirmPanel = false;
			}
			ImGui::EndPopup();
		}
	}

	ImGui::End();

	return true;
}

void HierarchyMenu::ShowChildren(GameObject* parent)
{
	GameObject* selected = app->editor->GetGO();
	for (int i = 0; i < parent->GetChilds().size(); ++i)
	{
		GameObject* obj = parent->GetChilds()[i];
		ImGui::PushID(obj->GetName());
		ImGuiTreeNodeFlags flags = SetFlags(obj);
		bool opened = false;
		if (obj != nullptr)
		{
			uint uuid = obj->GetUUID();
			opened = ImGui::TreeNodeEx((void*)obj, flags, obj->GetName());
			if (ImGui::BeginDragDropSource())
			{
				ImGui::Text("Moving \"%s\"", obj->name.c_str());
				ImGui::SetDragDropPayload("HierarchyItem", &uuid, sizeof(uint));
				//ImGui::SetDragDropPayload("HierarchyItemGameObject", &uuid, sizeof(uint));

				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* go = ImGui::AcceptDragDropPayload("HierarchyItem");
				if (go)
				{
					uint goUuid = *(const uint*)(go->Data);
					app->sceneManager->GetCurrentScene()->ReparentGameObjects(goUuid, obj);
				}
				ImGui::EndDragDropTarget();
			}
			if (ImGui::IsMouseReleased(0) && ImGui::IsItemHovered())
			{
				app->editor->SetGO(obj);
				app->editor->SetSelectedParent(parent);
			}
			else if (ImGui::IsItemClicked(1))
			{
				app->editor->SetGO(obj);
				app->editor->SetSelectedParent(parent);
				gameObjectOptions = true;
			}

			if (opened)
			{
				ShowChildren(obj);
				ImGui::TreePop();
			}

			if (!ImGui::IsAnyItemHovered() && (ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1]))
			{
				/*app->editor->SetSelected(nullptr);
				app->editor->SetSelectedParent(nullptr);*/
			}
		}
		ImGui::PopID();
	}
}
ImGuiTreeNodeFlags HierarchyMenu::SetFlags(GameObject* node)
{
	// This flags allow to open the tree if you click on arrow or doubleClick on object, by default the tree is open  
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	// If GameObject doesn't childrens = no collapsing and no arrow
	if (node->GetChilds().size() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	// If GameObject is selected = activeModeSelected
	if (node == app->editor->GetGO())
		flags |= ImGuiTreeNodeFlags_Selected;

	return flags;
}