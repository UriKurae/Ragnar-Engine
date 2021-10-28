#include "HierarchyMenu.h"

#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "GameObject.h"

HierarchyMenu::HierarchyMenu() : Menu(true)
{
	hierarchy = false;
	gameObjectOptions = false;
	createGameObject = false;
}

HierarchyMenu::~HierarchyMenu()
{
}

bool HierarchyMenu::Update(float dt)
{
	ImGui::Begin("Hierarchy", &hierarchy);
	if (ImGui::Button("+"))
	{
		createGameObject = true;
	}
	int size = app->scene->GetGameObjectsList().size();
	GameObject* root = app->scene->GetRoot();
	GameObject* selected = app->editor->GetSelected();
	GameObject* selectedParent = app->editor->GetSelectedParent();
	ImGuiTreeNodeFlags flags = ((selected == root) ? ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow;
	if (ImGui::TreeNodeEx(root, flags, root->GetName()))
	{
		ShowChildren(root);
		ImGui::TreePop();
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
				for (std::vector<GameObject*>::iterator i = selectedParent->GetChilds().begin(); i != selectedParent->GetChilds().end(); ++i)
				{
					if (selected == (*i))
					{
						selectedParent->GetChilds().erase(i);
						RELEASE(selected);
						break;
					}
				}
				app->editor->SetSelected(nullptr);
				gameObjectOptions = false;
			}
			else if (!ImGui::IsAnyItemHovered() && ((ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1])))
			{
				app->editor->SetSelected(nullptr);
				app->editor->SetSelectedParent(nullptr);
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
			if (ImGui::Button("Create Empty Object"))
			{
				if (selected != nullptr) app->scene->CreateGameObject(selected);
				else app->scene->CreateGameObject(nullptr);
				createGameObject = false;
			}
			else if (ImGui::Button("Create Cube"))
			{
				if (selected != nullptr) app->scene->Create3DObject(Object3D::CUBE, selected);
				else app->scene->Create3DObject(Object3D::CUBE, nullptr);
				createGameObject = false;
			}
			else if (ImGui::Button("Create Pyramide"))
			{
				if (selected != nullptr) app->scene->Create3DObject(Object3D::PYRAMIDE, selected);
				else app->scene->Create3DObject(Object3D::PYRAMIDE, nullptr);
				createGameObject = false;
			}
			else if (ImGui::Button("Create Sphere"))
			{
				if (selected != nullptr) app->scene->Create3DObject(Object3D::SPHERE, selected);
				else app->scene->Create3DObject(Object3D::SPHERE, nullptr);
				createGameObject = false;
			}
			else if (ImGui::Button("Create Cylinder"))
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
	int size = parent->GetChilds().size();
	GameObject* selected = app->editor->GetSelected();
	for (int i = 0; i < size; ++i)
	{
		GameObject* obj = parent->GetChilds()[i];
		ImGuiTreeNodeFlags flags = ((selected == obj) ? ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = false;
		if (obj != nullptr) opened = ImGui::TreeNodeEx((void*)obj, flags, obj->GetName());
		if (ImGui::IsItemClicked())
		{
			app->editor->SetSelected(obj);
			app->editor->SetSelectedParent(parent);
		}
		else if (ImGui::IsItemClicked(1))
		{
			app->editor->SetSelected(obj);
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
			app->editor->SetSelected(nullptr);
			app->editor->SetSelectedParent(nullptr);
		}
	}
}