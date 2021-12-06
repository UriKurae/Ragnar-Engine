#include "HierarchyMenu.h"

#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "GameObject.h"

#include "Profiling.h"

HierarchyMenu::HierarchyMenu() : Menu(true)
{
	gameObjectOptions = false;
	createGameObject = false;
}

HierarchyMenu::~HierarchyMenu()
{
}

bool HierarchyMenu::Update(float dt)
{
	ImGui::Begin("Hierarchy", &active, ImGuiWindowFlags_NoCollapse);
	if (ImGui::Button("+"))
	{
		createGameObject = true;
	}

	int size = app->scene->GetGameObjectsList().size();
	GameObject* root = app->scene->GetRoot();
	GameObject* selected = app->editor->GetGO();
	GameObject* selectedParent = app->editor->GetSelectedParent();
	ImGuiTreeNodeFlags flags = ((selected == root) ? ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow;
	if (ImGui::TreeNodeEx(root, flags, root->GetName()))
	{
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* go = ImGui::AcceptDragDropPayload("HierarchyItem");
			if (go)
			{
				uint goUuid = *(const uint*)(go->Data);
				app->scene->ReparentGameObjects(goUuid, root);
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
		ImGui::OpenPopup("GameObject");

		if (ImGui::BeginPopup("GameObject"))
		{
			if (ImGui::Button("Move Up", ImVec2(100.0f, 30.0f)))
			{
				if (selectedParent != nullptr)
				{
					selectedParent->MoveChildrenUp(selected);
				}
				else
				{
					app->scene->MoveGameObjectUp(selected);
				}
				gameObjectOptions = false;
			}
			else if (ImGui::Button("Move Down", ImVec2(100.0f, 30.0f)))
			{
				if (selectedParent != nullptr)
				{
					selectedParent->MoveChildrenDown(selected);
				}
				else
				{
					app->scene->MoveGameObjectDown(selected);
				}
				gameObjectOptions = false;
			}
			else if (ImGui::Button("Delete", ImVec2(100.0f, 30.0f)))
			{
				
				if (selected && selected->GetComponent<CameraComponent>() == nullptr)
				{
					for (std::vector<GameObject*>::iterator i = selectedParent->GetChilds().begin(); i != selectedParent->GetChilds().end(); ++i)
					{
						if (selected == (*i))
						{
							selectedParent->GetChilds().erase(i);
							if (selected == app->scene->GetRecalculateGO()) app->scene->RecalculateAABB(nullptr);
							RELEASE(selected);
							app->scene->ResetQuadtree();
							break;
						}
					}
				}
				app->editor->SetGO(nullptr);
				gameObjectOptions = false;
			}
			else if (!ImGui::IsAnyItemHovered() && ((ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1])))
			{
				/*app->editor->SetSelected(nullptr);
				app->editor->SetSelectedParent(nullptr);*/
				gameObjectOptions = false;
			}
			ImGui::EndPopup();
		}
	}
	else if (createGameObject)
	{
		ImGui::OpenPopup("Create GameObject");
		if (ImGui::BeginPopup("Create GameObject"))
		{
			if (ImGui::Selectable("Create Empty Object"))
			{
				if (selected != nullptr) app->scene->CreateGameObject(selected);
				else app->scene->CreateGameObject(nullptr);
				createGameObject = false;
			}
			else if (ImGui::Selectable("Create Cube"))
			{
				if (selected != nullptr) app->scene->Create3DObject(Object3D::CUBE, selected);
				else app->scene->Create3DObject(Object3D::CUBE, nullptr);
				createGameObject = false;
			}
			else if (ImGui::Selectable("Create Pyramide"))
			{
				if (selected != nullptr) app->scene->Create3DObject(Object3D::PYRAMIDE, selected);
				else app->scene->Create3DObject(Object3D::PYRAMIDE, nullptr);
				createGameObject = false;
			}
			else if (ImGui::Selectable("Create Sphere"))
			{
				if (selected != nullptr) app->scene->Create3DObject(Object3D::SPHERE, selected);
				else app->scene->Create3DObject(Object3D::SPHERE, nullptr);
				createGameObject = false;
			}
			else if (ImGui::Selectable("Create Cylinder"))
			{
				if (selected != nullptr) app->scene->Create3DObject(Object3D::CYLINDER, selected);
				else app->scene->Create3DObject(Object3D::CYLINDER, nullptr);
				createGameObject = false;
			}
			else if (!ImGui::IsAnyItemHovered() && ((ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1])))
			{
				createGameObject = false;
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
		ImGuiTreeNodeFlags flags = ((selected == obj) ? ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = false;
		if (obj != nullptr)
		{
			uint uuid = obj->GetUUID();
			opened = ImGui::TreeNodeEx((void*)obj, flags, obj->GetName());
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("HierarchyItem", &uuid, sizeof(uint));

				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* go = ImGui::AcceptDragDropPayload("HierarchyItem");
				if (go)
				{
					uint goUuid = *(const uint*)(go->Data);
					app->scene->ReparentGameObjects(goUuid, obj);
				}
				ImGui::EndDragDropTarget();
			}
			if (ImGui::IsItemClicked())
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