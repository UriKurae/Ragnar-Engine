#include "GameObject.h"

#include "Application.h"
#include "ModuleScene.h"
#include "Globals.h"

#include "JsonParsing.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "glew/include/GL/glew.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_internal.h"

#include "Profiling.h"


GameObject::GameObject() : active(true), parent(nullptr), name("Game Object"), newComponent(false), index(nullptr), vertex(nullptr), colliders(false), staticObj(true)
{
	globalAabb.SetNegativeInfinity();
	LCG lcg;
	uuid = lcg.IntFast();
}

GameObject::~GameObject()
{
	for (int i = 0; i < components.size(); ++i)
	{
		RELEASE(components[i]);
	}
	components.clear();

	for (int i = 0; i < children.size(); ++i)
	{
		RELEASE(children[i]);
	}
	children.clear();

	RELEASE(vertex);
	RELEASE(index);
}

bool GameObject::Update(float dt)
{
	for (int i = 0; i < components.size(); ++i)
		components[i]->Update(dt);

	for (int i = 0; i < children.size(); ++i)
		children[i]->Update(dt);

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
		Component* component = components[i];
		if (component->GetActive())
			component->Draw();
	}

	if (index && vertex && colliders)
	{
		DebugColliders();
	}
}

void GameObject::DrawOutline()
{
	for (int i = 0; i < components.size(); ++i)
	{
		Component* component = components[i];
		if (component->GetActive())
			component->DrawOutline();
	}

	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->DrawOutline();
	}
}

void GameObject::DrawEditor()
{
	ImGui::PushMultiItemsWidths(3, ImGui::GetWindowWidth());
	ImGui::Checkbox("Active", &active);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::InputText("Name", &name[0], 20);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::Checkbox("Static", &staticObj);
	ImGui::PopItemWidth();

	ImGui::Checkbox("Colliders", &colliders);
	ImGui::Separator();
	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->OnEditor();
	}

	ImGui::NewLine();
	float x = ImGui::GetWindowSize().x;
	ImGui::SameLine((x / 2) - 50);
	ImGui::SetNextItemWidth(120);
	if (ImGui::BeginCombo(" ", "New Component"))
	{
		if (ImGui::Selectable("Mesh Component"))
		{
			CreateComponent(ComponentType::MESH_RENDERER);
			newComponent = false;
		}
		if (ImGui::Selectable("Material Component"))
		{
			CreateComponent(ComponentType::MATERIAL);
			newComponent = false;
		}
		else if (!ImGui::IsAnyItemHovered() && ((ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1])))
		{
			newComponent = false;
		}
		//newComponent = true;
		ImGui::EndCombo();
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
			if (ImGui::Button("Material Component"))
			{
				CreateComponent(ComponentType::MATERIAL);
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

void GameObject::DebugColliders()
{
	//glPushMatrix();
	//
	//glMultMatrixf(GetComponent<TransformComponent>()->GetTransform().Transposed().ptr());

	glEnableClientState(GL_VERTEX_ARRAY);
	vertex->Bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	index->Bind();
	glLineWidth(2.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glDrawElements(GL_LINES, index->GetSize(), GL_UNSIGNED_INT, NULL);
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);
	vertex->Unbind();
	index->Unbind();
	glDisableClientState(GL_VERTEX_ARRAY);
	/*glPopMatrix();*/

	// TODO delete this when done

	// Configure buffers
	float3 corners[8];
	globalAabb.GetCornerPoints(corners);

	unsigned int indices[24] =
	{
		0,1,
		1,3,
		3,2,
		2,0,

		1,5,
		4,6,
		7,3,

		6,7,
		6,2,

		7,5,
		4,5,

		4,0
	};

	if (index) RELEASE(index);
	if (vertex) RELEASE(vertex);
	index = new IndexBuffer(indices, 24);
	vertex = new VertexBuffer(corners, sizeof(float3) * 8);
	index->Unbind();
	vertex->Unbind();
}

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* component = nullptr;

	switch (type)
	{
	case ComponentType::TRANSFORM:
		component = new TransformComponent(this);
		break;
	case ComponentType::MESH_RENDERER:
		component = new MeshComponent(this, GetComponent<TransformComponent>());
		break;
	case ComponentType::CAMERA:
		component = new CameraComponent(this, GetComponent<TransformComponent>());
		app->scene->SetMainCamera((CameraComponent*)component);
		break;
	case ComponentType::MATERIAL:
		component = new MaterialComponent(this);
		MeshComponent* m = GetComponent<MeshComponent>();
		if (m != nullptr) m->SetMaterial((MaterialComponent*)component);
		break;
	}

	if (component != nullptr)
	{
		component->SetOwner(this);
		components.push_back(component);
	}

	return component;
}

void GameObject::AddComponent(Component* component)
{
	component->SetOwner(this);
	components.emplace_back(component);
}

void GameObject::CopyComponent(Component* component)
{
	Component* c = nullptr;
	switch (component->type)
	{
	case ComponentType::TRANSFORM:
		c = new TransformComponent(dynamic_cast<TransformComponent*>(component));
		break;
	case ComponentType::MESH_RENDERER:
		c = new MeshComponent(dynamic_cast<MeshComponent*>(component), GetComponent<TransformComponent>());
		break;
	case ComponentType::MATERIAL:
		c = new MaterialComponent(dynamic_cast<MaterialComponent*>(component));
		MeshComponent* m = GetComponent<MeshComponent>();
		if (m != nullptr) m->SetMaterial((MaterialComponent*)c);
		break;
	}

	if (c != nullptr)
	{
		c->SetOwner(this);
		components.push_back(c);
	}
}

void GameObject::AddChild(GameObject* object)
{
	children.emplace_back(object);
}

void GameObject::RemoveChild(GameObject* object)
{
	for (std::vector<GameObject*>::iterator i = children.begin(); i != children.end(); ++i)
	{
		if ((*i) == object)
		{
			children.erase(i);
			break;
		}
	}
}

void GameObject::SetAABB(AABB newAABB, bool needToClean)
{
	globalObb = newAABB;
	globalObb.Transform(GetComponent<TransformComponent>()->GetGlobalTransform());

	globalAabb.Enclose(globalObb);

	if (parent != nullptr && parent != app->scene->GetRoot())
	{
		parent->SetAABB(globalAabb);
	}

	// Configure buffers
	float3 corners[8];
	globalAabb.GetCornerPoints(corners);
	
	unsigned int indices[24] = 
	{
		0,1,
		1,3,
		3,2,
		2,0,

		1,5,
		4,6,
		7,3,

		6,7,
		6,2,

		7,5,
		4,5,

		4,0
	};

	if (index) RELEASE(index);
	if (vertex) RELEASE(vertex);
	index = new IndexBuffer(indices, 24);
	vertex = new VertexBuffer(corners, sizeof(float3) * 8);
	index->Unbind();
	vertex->Unbind();
}

void GameObject::SetAABB(OBB newOBB)
{
	globalAabb.Enclose(newOBB);

	//if (parent != nullptr && parent != app->scene->GetRoot())
	//{
	//	OBB newObb = globalAabb.ToOBB();
	//	//newObb.Transform(GetComponent<TransformComponent>()->GetGlobalTransform());
	//	parent->SetAABB(newObb);
	//}
}

void GameObject::SetNewAABB()
{
	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->SetNewAABB();
		OBB newObb = children[i]->GetAABB().ToOBB();
		globalAabb.Enclose(newObb);
	}
	if (GetComponent<MeshComponent>() && GetComponent<MeshComponent>()->GetMesh())
	{
		OBB newObb = GetComponent<MeshComponent>()->GetLocalAABB().ToOBB();
		newObb.Transform(GetComponent<TransformComponent>()->GetGlobalTransform());
		globalAabb.Enclose(newObb);
	}
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

void GameObject::OnLoad(JsonParsing& node)
{
	uuid = node.GetJsonNumber("UUID");
	name = node.GetJsonString("Name");
	active = node.GetJsonBool("Active");

	JSON_Array* jsonArray = node.GetJsonArray(node.ValueToObject(node.GetRootValue()), "Components");

	size_t size = node.GetJsonArrayCount(jsonArray);
	for (int i = 0; i < size; ++i)
	{
		JsonParsing c = node.GetJsonArrayValue(jsonArray, i);
		Component* component = CreateComponent((ComponentType)(int)c.GetJsonNumber("Type"));
		component->OnLoad(c);
	}
}

void GameObject::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "UUID", uuid);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Parent UUID", parent ? parent->GetUUID() : 0);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Name", name.c_str());
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);

	JSON_Array* newArray = file.SetNewJsonArray(file.GetRootValue(), "Components");

	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->OnSave(file, newArray);
	}

	node.SetValueToArray(array, file.GetRootValue());

	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->OnSave(node, array);
	}
}