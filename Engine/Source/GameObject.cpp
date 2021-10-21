#include "GameObject.h"
#include "Globals.h"

#include "Imgui/imgui.h"

#include "mmgr/mmgr.h"

GameObject::GameObject() : active(true), parent(nullptr), name("Game Object"), newComponent(false)
{
}

GameObject::~GameObject()
{
	for (int i = 0; i < components.size(); ++i)
	{
		RELEASE(components[i]);
	}
}

bool GameObject::Update(float dt)
{
	return true;
}

void GameObject::Draw()
{
	// TODO: Check this in the future
	//if (!GetAllComponent<MeshComponent>().empty())
	//{
	//	for (int i = 0; i < GetAllComponent<MeshComponent>().size(); ++i)
	//	{
	//		GetAllComponent<MeshComponent>()[i]->Draw();
	//	}
	//}

	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->Draw();
	}
}

void GameObject::DrawEditor()
{
	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->OnEditor();
	}

	ImGui::NewLine();
	float x = ImGui::GetWindowSize().x;
	ImGui::SameLine((x / 2) - 50);
	if (ImGui::Button("New Component", ImVec2(100, 20)))
	{
		newComponent = true;
	}

	if (newComponent)
	{
		ImGui::OpenPopup("New Component");
		if (ImGui::BeginPopup("New Component"))
		{
			if (ImGui::Button("Mesh Component"))
			{
				CreateComponent(ComponentType::MESH_RENDERER);
				newComponent = false;
			}
			else if (!ImGui::IsAnyItemHovered() && ((ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1])))
			{
				newComponent = false;
			}
			ImGui::EndPopup();
		}
	}
}

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* component = nullptr;

	switch (type)
	{
	case ComponentType::TRANSFORM:
		component = new TransformComponent();
		break;
	case ComponentType::MESH_RENDERER:
		/*component = new MeshComponent(GetComponent<TransformComponent>());*/
		break;
	case ComponentType::MATERIAL:

		break;
	}

	if (component != nullptr) components.push_back(component);

	return component;
}

void GameObject::AddComponent(Component* component)
{
	component->SetOwner(this);
	components.emplace_back(component);
}

void GameObject::AddChild(GameObject* object)
{
	children.emplace_back(object);
}

void GameObject::MoveChildrenUp(GameObject* child)
{
	if (child == children[0]) return;

	int size = children.size();
	for (int i = 0; i < size; ++i)
	{
		if (children[i] == child)
		{
			GameObject* aux = child;

			children[i] = children[i - 1];
			children[i - 1] = aux;
			break;
		}
	}
}

void GameObject::MoveChildrenDown(GameObject* child)
{
	int size = children.size() - 1;
	if (child == children[size]) return;

	for (int i = size; i >= 0; --i)
	{
		if (children[i] == child)
		{
			GameObject* aux = child;

			children[i] = children[i + 1];
			children[i + 1] = aux;
			break;
		}
	}
}