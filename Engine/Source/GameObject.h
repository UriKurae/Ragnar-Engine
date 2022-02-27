#pragma once

#include <string>
#include <vector>
#include "Component.h"
#include "Geometry/OBB.h"

#include "TransformComponent.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"
#include "AudioSourceComponent.h"
#include "ListenerComponent.h"
#include "AudioReverbZoneComponent.h"

typedef unsigned int uint;

typedef json_array_t JSON_Array;
class JsonParsing;
class VertexBuffer;
class IndexBuffer;

class GameObject
{
public:
	GameObject();
	~GameObject();

	bool Update(float dt);
	void Draw();
	void DrawOutline();
	void DrawEditor();

	Component* CreateComponent(ComponentType type);
	void AddComponent(Component* component);
	void RemoveComponent(Component* component);
	void MoveComponent(Component* component, int position);

	void CopyComponent(Component* component);
	
	void AddChild(GameObject* object);
	void RemoveChild(GameObject* object);
	void RemoveChildren(std::vector<GameObject*>::const_iterator i) { children.erase(i); };

	std::vector<GameObject*>::const_iterator FindChildren(GameObject* child) { return std::find(children.begin(), children.end(), child); };

	inline void SetParent(GameObject* object) { parent = object; }
	inline void SetName(const char* n) { name = n; }
	inline void SetAudioRegister(bool check) { audioRegistered = check; }

	inline uint const GetUUID() const { return uuid; }
	inline const char* GetName() const { return name.c_str(); }
	inline GameObject* GetParent() const { return parent; }
	inline const bool& GetActive() const { return active; }
	inline std::vector<GameObject*>& GetChilds() { return children; }
	inline bool CheckAudioRegister() { return audioRegistered; }

	void SetAABB(AABB newAABB, bool needToClean = false);
	void SetAABB(OBB newOBB);
	void SetNewAABB();
	inline AABB GetAABB() { return globalAabb; }
	inline OBB GetOOB() { return globalObb; }
	
	inline void ClearAABB() { globalAabb.SetNegativeInfinity(); }

	void MoveChildrenUp(GameObject *child);
	void MoveChildrenDown(GameObject *child);

	void OnLoad(JsonParsing& node);
	void OnSave(JsonParsing& node, JSON_Array* array);

	inline const std::vector<Component*> GetComponents() const { return components; }

	template<typename T>
	T* GetComponent();
	
public:
	std::string name;
	bool active;
	bool staticObj;
	std::string tag;
	std::string layer;

private:
	std::vector<Component*> components;

	GameObject* parent;
	std::vector<GameObject*> children;

	bool newComponent;

	AABB globalAabb;
	OBB globalObb;

	uint uuid;

	//MouseMoveCommand mouseMoveCommand;

	// Boolean to check if any component from audio (AudioSource, listener) has already been registered. 
	bool audioRegistered;
};

template<typename T>
inline T* GameObject::GetComponent()
{
	T* component = nullptr;
	
	for (std::vector<Component*>::iterator i = components.begin(); i < components.end(); ++i)
	{
		component = dynamic_cast<T*>(*i);
		if (component != nullptr)
			return component;
	}

	return component;
}