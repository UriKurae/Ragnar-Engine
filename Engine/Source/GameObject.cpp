#include "GameObject.h"
#include "Application.h"
#include "Globals.h"
#include "ParticleSystemComponent.h"

#include "ModuleScene.h"

#include "MonoManager.h"
#include "JsonParsing.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Component.h"
#include "C_RigidBody.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "LightComponent.h"
#include "AudioSourceComponent.h"
#include "ListenerComponent.h"
#include "AudioReverbZoneComponent.h"
#include "ScriptComponent.h"
#include "AnimationComponent.h"

#include "Algorithm/Random/LCG.h"
#include "Profiling.h"

GameObject::GameObject() : active(true), parent(nullptr), name("Game Object"), newComponent(false), staticObj(true), audioRegistered(false), tag("Untagged"), layer("Default"), prefabPath("None"), prefabID(0)
{
	globalAabb.SetNegativeInfinity();
	LCG lcg;
	uuid = lcg.Int();
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
}

bool GameObject::Update(float dt)
{
	for (int i = 0; i < components.size(); ++i)
		components[i]->Update(dt);

	for (int i = 0; i < children.size(); ++i)
		children[i]->Update(dt);

	return true;
}

void GameObject::Draw(CameraComponent* gameCam)
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
		{
			component->Draw(gameCam);
		}
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
		if (ImGui::Selectable("Script Component"))
		{
			CreateComponent(ComponentType::SCRIPT);
			newComponent = false;
		}
	
		if (ImGui::Selectable("Audio Source Component"))
		{
			CreateComponent(ComponentType::AUDIO_SOURCE);
			newComponent = false;
		}
		if (ImGui::Selectable("Audio Listener Component"))
		{
			CreateComponent(ComponentType::AUDIO_LISTENER);
			newComponent = false;
		}
		if (ImGui::Selectable("Audio Reverb Zone Component"))
		{
			CreateComponent(ComponentType::AUDIO_REVERB_ZONE);
			newComponent = false;
		}
		if (ImGui::Selectable("Particle System Component"))
		{
			CreateComponent(ComponentType::PARTICLE_SYSTEM);
			newComponent = false;
		}
		if (ImGui::Selectable("Animation Component"))
		{
			CreateComponent(ComponentType::ANIMATION);
			newComponent = false;
		}
		if (ImGui::Selectable("Rigid Body"))
		{
			CreateComponent(ComponentType::RIGID_BODY);
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
			if (ImGui::Button("Script Component"))
			{
				CreateComponent(ComponentType::SCRIPT);
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


Component* GameObject::CreateComponent(ComponentType type, const char* name)
{
	Component* component = nullptr;
	TransformComponent* transform = nullptr;

	switch (type)
	{
	case ComponentType::TRANSFORM:
		component = new TransformComponent(this);
		break;
	case ComponentType::MESH_RENDERER:
		component = new MeshComponent(this, GetComponent<TransformComponent>());
// <<<<<<< HEAD
// 		{
// 			MeshComponent* meshComp = (MeshComponent*)component;

// 			if (meshComp)
// 			{
// 				MaterialComponent* matComp = new MaterialComponent(this);
// 				meshComp->SetMaterial((MaterialComponent*)matComp);

// 				if (matComp)
// 				{
// 					matComp->SetOwner(this);
// 					components.push_back(matComp);
// 				}
// =======
		
		{
			MeshComponent* meshComp = (MeshComponent*)component;

			MaterialComponent* matComp = GetComponent<MaterialComponent>();
			if (matComp != nullptr)
			{
				meshComp->SetMaterial(matComp);
			}
			else
			{
				matComp = new MaterialComponent(this, true);
				meshComp->SetMaterial(matComp);
				matComp->SetOwner(this);
				components.push_back(matComp);
				//matComp = (MaterialComponent*)CreateComponent(ComponentType::MATERIAL);
			}
		}
		break;
	case ComponentType::SCRIPT:
		component = new ScriptComponent(this, name);
		break;
	case ComponentType::CAMERA:
		component = new CameraComponent(this, GetComponent<TransformComponent>());
		app->scene->SetMainCamera((CameraComponent*)component);
		break;
	case ComponentType::AUDIO_SOURCE:
		component = new AudioSourceComponent(this, GetComponent<TransformComponent>());
		break;
	case ComponentType::AUDIO_LISTENER:
		component = new ListenerComponent(this, GetComponent<TransformComponent>());
		break;
	case ComponentType::AUDIO_REVERB_ZONE:
		component = new AudioReverbZoneComponent(this, GetComponent<TransformComponent>());
		break;
	case ComponentType::ANIMATION:
		component = new AnimationComponent(this);
		break;
	case ComponentType::RIGID_BODY:
		component = new RigidBodyComponent(this);
		break;
	case ComponentType::MATERIAL:
	{
		{
			MaterialComponent* matComp = GetComponent<MaterialComponent>();
			if (matComp != nullptr && matComp->IsDefaultMat())
			{
				std::vector<Component*>::iterator it = components.begin();
				for (; it != components.end(); ++it)
				{
					if (*(it) == matComp)
					{
						components.erase(it);
						RELEASE(matComp);
						break;
					}
				}

				component = new MaterialComponent(this, false);
			}
			else
			{
				component = new MaterialComponent(this, false);
			}

			MeshComponent* m = GetComponent<MeshComponent>();
			if (m != nullptr)
				m->SetMaterial((MaterialComponent*)component);
		}

		break;
	}
	case ComponentType::LIGHT:
		component = new ComponentLight();
		break;
	
	case ComponentType::PARTICLE_SYSTEM:
		transform = (TransformComponent*)GetComponent<TransformComponent>();
		component = new ParticleSystemComponent(this, transform);
		break;
	case ComponentType::BILLBOARD:
		transform = (TransformComponent*)GetComponent<TransformComponent>();
		component = new BillboardParticleComponent(this, transform);
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

void GameObject::RemoveComponent(Component* component)
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it) == component)
		{
			components.erase(it);
			RELEASE(component);
			break;
		}
	}
}

void GameObject::MoveComponent(Component* component, int position)
{
	//TODO: Add to each component the reorganitation structure where the delete button is
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if (*it == component && (it - position) > components.begin() && (it - position) < components.end())
		{
			std::swap(*it, *(it - position));
			break;
		}
	}
}

void GameObject::CopyComponent(Component* component)
{
	//TODO: Copy every single type of Components
	Component* c = nullptr;
	switch (component->type)
	{
	case ComponentType::TRANSFORM:
		c = new TransformComponent(dynamic_cast<TransformComponent*>(component));
		break;
	case ComponentType::MESH_RENDERER:
		c = new MeshComponent(dynamic_cast<MeshComponent*>(component), GetComponent<TransformComponent>());
		break;
	case ComponentType::ANIMATION:
		c = new AnimationComponent(dynamic_cast<AnimationComponent*>(component));
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
	object->parent = this;
	children.emplace_back(object);
	TransformComponent* trans = object->GetComponent<TransformComponent>();
	if (object->parent != nullptr && trans) trans->NewAttachment();
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

	globalAabb.SetNegativeInfinity();
	globalAabb.Enclose(globalObb);
}

void GameObject::SetAABB(OBB newOBB)
{
	globalObb = newOBB;
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
	prefabID = node.GetJsonNumber("PrefabID");
	prefabPath = node.GetJsonString("Prefab Path");

	JSON_Array* jsonArray = node.GetJsonArray(node.ValueToObject(node.GetRootValue()), "Components");

	size_t size = node.GetJsonArrayCount(jsonArray);
	for (int i = 0; i < size; ++i)
	{
		JsonParsing c = node.GetJsonArrayValue(jsonArray, i);
		Component* component = CreateComponent((ComponentType)(int)c.GetJsonNumber("Type"), c.GetJsonString("ScriptName"));
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
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "PrefabID", prefabID);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Prefab Path", prefabPath.c_str());

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

void GameObject::OnSavePrefab(JsonParsing& node, JSON_Array* array, int option)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "UUID", uuid);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Parent UUID", parent ? parent->GetUUID() : 0);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Name", name.c_str());
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Prefab Path", prefabPath.c_str());

	if (option == 1 || option == 3)
	{
		LCG lcg;
		prefabID = lcg.Int();
	}
	if (option == 2 && prefabID == 0)
	{
		LCG lcg;
		prefabID = lcg.Int();
	}

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PrefabID", prefabID);

	JSON_Array* newArray = file.SetNewJsonArray(file.GetRootValue(), "Components");

	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->OnSave(file, newArray);
	}

	node.SetValueToArray(array, file.GetRootValue());

	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->OnSavePrefab(node, array, option);
	}
}

void GameObject::UpdateFromPrefab(JsonParsing& node, bool isParent)
{
	active = node.GetJsonBool("Active");

	JSON_Array* jsonArray = node.GetJsonArray(node.ValueToObject(node.GetRootValue()), "Components");

	size_t size = node.GetJsonArrayCount(jsonArray);

	std::vector<ComponentType> listComp;

	for (int i = 0; i < size; ++i)
	{
		JsonParsing c = node.GetJsonArrayValue(jsonArray, i);
		ComponentType comp = (ComponentType)(int)c.GetJsonNumber("Type");

		listComp.push_back(comp);

		switch (comp)
		{
		case ComponentType::TRANSFORM:
			if (!isParent)
			{
				if (GetComponent<TransformComponent>() == nullptr)
					CreateComponent(ComponentType::TRANSFORM);

				GetComponent<TransformComponent>()->OnLoad(c);
			}
			break;
		case ComponentType::MESH_RENDERER:
			if (GetComponent<MeshComponent>() == nullptr)
				CreateComponent(ComponentType::MESH_RENDERER);

			GetComponent<MeshComponent>()->OnLoad(c);
			break;
		case ComponentType::CAMERA:
			if (GetComponent<CameraComponent>() == nullptr)
				CreateComponent(ComponentType::CAMERA);

			GetComponent<CameraComponent>()->OnLoad(c);
			break;
		case ComponentType::AUDIO_SOURCE:
			if (GetComponent<AudioSourceComponent>() == nullptr)
				CreateComponent(ComponentType::AUDIO_SOURCE);

			GetComponent<AudioSourceComponent>()->OnLoad(c);
			break;
		case ComponentType::AUDIO_LISTENER:
			if (GetComponent<ListenerComponent>() == nullptr)
				CreateComponent(ComponentType::AUDIO_LISTENER);

			GetComponent<ListenerComponent>()->OnLoad(c);
			break;
		case ComponentType::AUDIO_REVERB_ZONE:
			if (GetComponent<AudioReverbZoneComponent>() == nullptr)
				CreateComponent(ComponentType::AUDIO_REVERB_ZONE);

			GetComponent<AudioReverbZoneComponent>()->OnLoad(c);
			break;
		case ComponentType::MATERIAL:
			if (GetComponent<MaterialComponent>() == nullptr)
				CreateComponent(ComponentType::MATERIAL);

			GetComponent<MaterialComponent>()->OnLoad(c);
			break;
		case ComponentType::LIGHT:
			if (GetComponent<ComponentLight>() == nullptr)
				CreateComponent(ComponentType::LIGHT);

			GetComponent<ComponentLight>()->OnLoad(c);
			break;
		case ComponentType::RIGID_BODY:
			if (GetComponent<RigidBodyComponent>() == nullptr)
				CreateComponent(ComponentType::RIGID_BODY);

			GetComponent<RigidBodyComponent>()->OnLoad(c);
			break;
		case ComponentType::SCRIPT:
			if (GetComponent<ScriptComponent>() == nullptr)
				CreateComponent(ComponentType::SCRIPT, c.GetJsonString("ScriptName"));

			GetComponent<ScriptComponent>()->OnLoad(c);
			break;
		}
	}

	std::vector<ComponentType> compToDelete;

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		bool exist = false;
		for (std::vector<ComponentType>::iterator it2 = listComp.begin(); it2 != listComp.end(); ++it2)
		{
			if ((*it)->type == (*it2))
			{
				exist = true;
				break;
			}
		}

		if (!exist)
		{
			compToDelete.push_back((*it)->type);
		}
	}

	for (std::vector<ComponentType>::iterator it = compToDelete.begin(); it != compToDelete.end(); ++it)
	{
		switch ((*it))
		{
		case ComponentType::TRANSFORM:
			RemoveComponent(GetComponent<TransformComponent>());
			break;
		case ComponentType::MESH_RENDERER:
			RemoveComponent(GetComponent<MeshComponent>());
			break;
		case ComponentType::CAMERA:
			RemoveComponent(GetComponent<CameraComponent>());
			break;
		case ComponentType::AUDIO_SOURCE:
			RemoveComponent(GetComponent<AudioSourceComponent>());
			break;
		case ComponentType::AUDIO_LISTENER:
			RemoveComponent(GetComponent<ListenerComponent>());
			break;
		case ComponentType::AUDIO_REVERB_ZONE:
			RemoveComponent(GetComponent<AudioReverbZoneComponent>());
			break;
		case ComponentType::MATERIAL:
			RemoveComponent(GetComponent<MaterialComponent>());
			break;
		case ComponentType::LIGHT:
			RemoveComponent(GetComponent<ComponentLight>());
			break;
		case ComponentType::RIGID_BODY:
			RemoveComponent(GetComponent<RigidBodyComponent>());
			break;
		case ComponentType::SCRIPT:
			RemoveComponent(GetComponent<ScriptComponent>());
			break;
		}
	}
}