#include "GameObject.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleSceneManager.h"
#include "Scene.h"

#include "ParticleSystemComponent.h"
#include "C_RigidBody.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "LightComponent.h"
#include "AudioSourceComponent.h"
#include "ListenerComponent.h"
#include "AudioReverbZoneComponent.h"
#include "ScriptComponent.h"
#include "InputActionComponent.h"
#include "AnimationComponent.h"
#include "BillboardParticleComponent.h"
#include "ButtonComponent.h"
#include "DropDownComponent.h"
#include "SliderComponent.h"
#include "ImageComponent.h"
#include "CheckBoxComponent.h"
#include "Transform2DComponent.h"
#include"TextComponent.h"
#include "NavAgentComponent.h"

#include "GL/glew.h"
#include "Algorithm/Random/LCG.h"
#include "Profiling.h"

GameObject::GameObject() : active(true), parent(nullptr), name("Game Object"), newComponent(false), staticObj(true), audioRegistered(false), tag("Untagged"), layer("Default"), prefabPath("None"), prefabID(0)
{
	globalAabb.SetNegativeInfinity();
	LCG lcg;
	uuid = lcg.Int();
	isInteractuable = false;
}

GameObject::~GameObject()
{
	// Delete all components
	for (int i = 0; i < components.size(); ++i)
	{
		RELEASE(components[i]);		
	}
	components.clear();

	if (isUI)
		app->userInterface->DeleteUIGameObjects(this);

	// Recursive Release, delete all childrens
	for (int i = 0; i < children.size(); ++i)
	{
		RELEASE(children[i]);
	}
	children.clear();
}

bool GameObject::Update(float dt)
{
	RG_PROFILING_FUNCTION("Game Object Update");

	for (int i = 0; i < components.size() && components[i]->active; ++i)
	{
		if (app->sceneManager->newSceneLoaded)
		{
			components[i]->Update(dt);
		}

		if (app->sceneManager->GetCurrentScene()->mainCamera->GetFrustum()->Intersects(globalAabb) || !globalAabb.IsFinite() || tag == "Player" || tag == "Ability" || name == "Boss" || components[i]->type == ComponentType::PARTICLE_SYSTEM)
		{
			hasBeenUpdate = true;
			components[i]->Update(dt);
		}
		else
			hasBeenUpdate = false;
	}

	for (int i = 0; i < children.size(); ++i)
		children[i]->Update(dt);

	return true;
}

void GameObject::Draw(CameraComponent* gameCam)
{
	for (int i = 0; i < components.size(); ++i)
	{
		Component* component = components[i];
		if (component->GetActive())
		{
			component->Draw(gameCam);
		}
	}

	if (showAABB == true)
	{
		if (gameCam)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(gameCam->matrixProjectionFrustum.Transposed().ptr());
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(gameCam->matrixViewFrustum.Transposed().ptr());
		}
		else
		{
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(app->camera->matrixProjectionFrustum.Transposed().ptr());
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(app->camera->matrixViewFrustum.Transposed().ptr());
		}

		float3 points[8];
		globalAabb.GetCornerPoints(points);
		DebugColliders(points, float3(0.2f, 1.f, 0.101f));

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	if (showOBB == true)
	{
		if (gameCam)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(gameCam->matrixProjectionFrustum.Transposed().ptr());
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(gameCam->matrixViewFrustum.Transposed().ptr());
		}
		else
		{
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(app->camera->matrixProjectionFrustum.Transposed().ptr());
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(app->camera->matrixViewFrustum.Transposed().ptr());
		}
		
		float3 points[8];
		globalObb.GetCornerPoints(points);
		DebugColliders(points);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}

void GameObject::DrawOutline(CameraComponent* gameCam, const float3& color)
{
	if (MeshComponent* border = GetComponent<MeshComponent>())
		border->DrawOutline(gameCam, color);
}

void GameObject::DrawEditor()
{	
	float x = ImGui::GetWindowSize().x;
	ImGui::SameLine((x / 2) - 50);
	ImGui::SetNextItemWidth(120);
	if (ImGui::BeginCombo(" ", "New Component"))
	{
		if (ImGui::Selectable("Animation Component"))
		{
			CreateComponent(ComponentType::ANIMATION);
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
		if (ImGui::Selectable("Audio Source Component"))
		{
			CreateComponent(ComponentType::AUDIO_SOURCE);
			newComponent = false;
		}
		if (ImGui::Selectable("Material Component"))
		{
			CreateComponent(ComponentType::MATERIAL);
			newComponent = false;
		}
		if (ImGui::Selectable("Mesh Component"))
		{
			CreateComponent(ComponentType::MESH_RENDERER);
			newComponent = false;
		}
		if (ImGui::Selectable("NavAgent"))
		{
			CreateComponent(ComponentType::NAVAGENT);
			newComponent = false;
		}
		if (ImGui::Selectable("Particle System Component"))
		{
			CreateComponent(ComponentType::PARTICLE_SYSTEM);
			newComponent = false;
		}
		if (ImGui::Selectable("Rigid Body"))
		{
			CreateComponent(ComponentType::RIGID_BODY);
			newComponent = false;
		}
		if (ImGui::Selectable("Script Component"))
		{
			CreateComponent(ComponentType::SCRIPT);
			newComponent = false;
		}
		if (ImGui::Selectable("Input Component"))
		{
			CreateComponent(ComponentType::INPUT_ACTION);
			newComponent = false;
		}
		/*if (ImGui::Selectable("Billboard Component"))
		{
			CreateComponent(ComponentType::BILLBOARD);
			newComponent = false;
		}*/
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
	TransformComponent* transform = GetComponent<TransformComponent>();
	
	switch (type)
	{
	case ComponentType::TRANSFORM:
		component = new TransformComponent(this);
		break;
	case ComponentType::MESH_RENDERER:
	{ // {} are necessary if you want declare variables into the case
		component = new MeshComponent(this, transform);

		MeshComponent* meshComp = (MeshComponent*)component;
		MaterialComponent* matComp = GetComponent<MaterialComponent>();

		if (matComp != nullptr)
			meshComp->SetMaterial(matComp);
		else
		{
			matComp = new MaterialComponent(this, true);
			meshComp->SetMaterial(matComp);
			matComp->SetOwner(this);
			components.push_back(matComp);
		}
	}		
		break;
	case ComponentType::SCRIPT:
		component = new ScriptComponent(this, name);
		break;
	case ComponentType::UI_BUTTON:
		component = new ButtonComponent(this);
		break;
	case ComponentType::UI_SLIDER:
		component = new SliderComponent(this);
		break;
	case ComponentType::UI_CHECKBOX:
		component = new CheckboxComponent(this);
		break;
	case ComponentType::UI_DROPDOWN:
		component = new DropDownComponent(this);
		break;
	case ComponentType::UI_IMAGE:
		component = new ImageComponent(this);
		break;
	case ComponentType::UI_TEXT:
		component = new TextComponent(this);
		break;
	case ComponentType::CAMERA:
		component = new CameraComponent(this, transform);
		app->sceneManager->GetCurrentScene()->SetMainCamera((CameraComponent*)component);
		break;
	case ComponentType::AUDIO_SOURCE:
		component = new AudioSourceComponent(this, transform);
		break;
	case ComponentType::AUDIO_LISTENER:
		component = new ListenerComponent(this, transform);
		break;
	case ComponentType::AUDIO_REVERB_ZONE:
		component = new AudioReverbZoneComponent(this, transform);
		break;
	case ComponentType::INPUT_ACTION:
		component = new InputActionComponent(this);
		break;
	case ComponentType::ANIMATION:
		component = new AnimationComponent(this);
		break;
	case ComponentType::RIGID_BODY:
		component = new RigidBodyComponent(this);
    	break;
	case ComponentType::NAVAGENT:
		component = new NavAgentComponent(this);
		break;
	case ComponentType::MATERIAL:
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
	case ComponentType::LIGHT:
		component = new ComponentLight(this);
		break;
	case ComponentType::PARTICLE_SYSTEM:
		component = new ParticleSystemComponent(this, transform);
		break;
	case ComponentType::BILLBOARD:
		component = new BillboardParticleComponent(this, transform);
		break;
	case ComponentType::TRANFORM2D:
		component = new ComponentTransform2D(this);
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
			if (GetComponent<MeshComponent>() == nullptr && GetComponent<ParticleSystemComponent>() == nullptr)
				app->sceneManager->GetCurrentScene()->RedistributeQuadtree(this);
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

void GameObject::AddChild(GameObject* object, bool begin)
{
	object->parent = this;
	if(!begin) children.emplace_back(object);
	else children.insert(children.begin(), object);
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
	globalAabb.Enclose(globalObb);
}

void GameObject::SetNewAABB()
{
	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->SetNewAABB();
		OBB newObb = children[i]->GetAABB().ToOBB();
		globalAabb.Enclose(newObb);
	}

	MeshComponent* mesh = GetComponent<MeshComponent>();
	if (mesh && mesh->GetMesh())
	{
		OBB newObb = mesh->GetLocalAABB().ToOBB();
		newObb.Transform(GetComponent<TransformComponent>()->GetGlobalTransform());
		globalAabb.Enclose(newObb);
	}
}

void GameObject::EditAABB(float3 offset, float3 size)
{
	globalAabb.SetFromCenterAndSize(GetComponent<TransformComponent>()->GetPosition() + offset, size);
	globalObb.SetFrom(globalAabb);
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
	staticObj = node.GetJsonBool("Static");
	prefabID = node.GetJsonNumber("PrefabID");
	prefabPath = node.GetJsonString("Prefab Path");
	tag = node.GetJsonString("tag");
	layer = node.GetJsonString("layer");
	isInteractuable = node.GetJsonBool("Is Interactuable");

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
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Static", staticObj);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "PrefabID", prefabID);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Prefab Path", prefabPath.c_str());
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "tag", tag.c_str());
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "layer", layer.c_str());
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Is Interactuable", isInteractuable);

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
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Static", staticObj);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Prefab Path", prefabPath.c_str());
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "tag", tag.c_str());
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "layer", layer.c_str());

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
		ComponentType compType = (ComponentType)(int)c.GetJsonNumber("Type");

		listComp.push_back(compType);

		switch (compType)
		{
		case ComponentType::TRANSFORM:
			if (!isParent)
			{
				TransformComponent* transform = GetComponent<TransformComponent>();
				if (transform == nullptr)
					CreateComponent(ComponentType::TRANSFORM);

				transform->OnLoad(c);
			}
			break;
		case ComponentType::MESH_RENDERER:
		{
			MeshComponent* comp = GetComponent<MeshComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::MESH_RENDERER);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::CAMERA:
		{
			CameraComponent* comp = GetComponent<CameraComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::CAMERA);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::AUDIO_SOURCE:
		{
			AudioSourceComponent* comp = GetComponent<AudioSourceComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::AUDIO_SOURCE);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::AUDIO_LISTENER:
		{
			ListenerComponent* comp = GetComponent<ListenerComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::AUDIO_LISTENER);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::AUDIO_REVERB_ZONE:
		{
			AudioReverbZoneComponent* comp = GetComponent<AudioReverbZoneComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::AUDIO_REVERB_ZONE);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::MATERIAL:
		{
			MaterialComponent* comp = GetComponent<MaterialComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::MATERIAL);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::LIGHT:
		{
			ComponentLight* comp = GetComponent<ComponentLight>();
			if (comp == nullptr)
				CreateComponent(ComponentType::LIGHT);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::RIGID_BODY:
		{
			RigidBodyComponent* comp = GetComponent<RigidBodyComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::RIGID_BODY);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::SCRIPT:
		{
			ScriptComponent* comp = GetComponent<ScriptComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::SCRIPT, c.GetJsonString("ScriptName"));

			comp->OnLoad(c);
		}
		break;
		case ComponentType::ANIMATION:
		{
			AnimationComponent* comp = GetComponent<AnimationComponent>();
			if (comp == nullptr)
			{
				CreateComponent(ComponentType::ANIMATION);
			}

			comp->OnLoad(c);
		}
		break;
		case ComponentType::INPUT_ACTION:
		{
			InputActionComponent* comp = GetComponent<InputActionComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::INPUT_ACTION);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::BILLBOARD:
		{
			BillboardParticleComponent* comp = GetComponent<BillboardParticleComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::BILLBOARD);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::PARTICLE_SYSTEM:
		{
			ParticleSystemComponent* comp = GetComponent<ParticleSystemComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::ANIMATION);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::UI_BUTTON:
		{
			ButtonComponent* comp = GetComponent<ButtonComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::UI_BUTTON);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::UI_CHECKBOX:
		{
			CheckboxComponent* comp = GetComponent<CheckboxComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::UI_CHECKBOX);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::UI_IMAGE:
		{
			ImageComponent* comp = GetComponent<ImageComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::UI_BUTTON);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::UI_SLIDER:
		{
			SliderComponent* comp = GetComponent<SliderComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::UI_BUTTON);

			comp->OnLoad(c);
		}
		break;
		case ComponentType::UI_TEXT:
		{
			TextComponent* comp = GetComponent<TextComponent>();
			if (comp == nullptr)
				CreateComponent(ComponentType::UI_TEXT);

			comp->OnLoad(c);
		}
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
		case ComponentType::ANIMATION:
			RemoveComponent(GetComponent<AnimationComponent>());
			break;
		case ComponentType::INPUT_ACTION:
			RemoveComponent(GetComponent<InputActionComponent>());
			break;
		case ComponentType::BILLBOARD:
			RemoveComponent(GetComponent<BillboardParticleComponent>());
			break;
		case ComponentType::PARTICLE_SYSTEM:
			RemoveComponent(GetComponent<ParticleSystemComponent>());
			break;
		case ComponentType::UI_BUTTON:
			RemoveComponent(GetComponent<ButtonComponent>());
			break;
		case ComponentType::UI_CHECKBOX:
			RemoveComponent(GetComponent<CheckboxComponent>());
			break;
		case ComponentType::UI_SLIDER:
			RemoveComponent(GetComponent<SliderComponent>());
			break;
		case ComponentType::UI_IMAGE:
			RemoveComponent(GetComponent<ImageComponent>());
			break;
		case ComponentType::UI_TEXT:
			RemoveComponent(GetComponent<TextComponent>());
			break;
		}
	}
}

void GameObject::EnableDisableActive(bool ret)
{
	active = ret;
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->EnableDisableActive(ret);
	}
}

void GameObject::EnableDisableStatic(bool ret)
{
	staticObj = ret;
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->EnableDisableStatic(ret);
	}
}

Component* GameObject::GetComponent(ComponentType type)
{
	for (auto& comp : components)
	{
		if (comp->type == type)
			return comp;
	}
}

void GameObject::DebugColliders(float3* points, float3 color)
{
	static unsigned int index[24] =
	{ 0, 2, 2, 6, 6, 4, 4, 0,
	  0, 1, 1, 3, 3, 2, 4, 5,
	  6, 7, 5, 7, 3, 7, 1, 5
	};

	glColor3fv(&color.x);
	glLineWidth(2.f);
	glBegin(GL_LINES);

	for (int i = 0; i < 24; i++)
	{
		glVertex3fv(&points[index[i]].x);
	}

	glEnd();
	glLineWidth(1.f);
	glColor3f(1.f, 1.f, 1.f);
}