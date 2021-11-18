#pragma once

#include "Module.h"
#include "GameObject.h"
#include "Quadtree.h"
#include "EngineTimer.h"
#include <vector>

enum class Object3D
{
	CUBE = 0,
	PYRAMIDE,
	SPHERE,
	CYLINDER
};

class CameraComponent;

class ModuleScene : public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Start() override;
	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool Draw();
	bool CleanUp() override;

	GameObject* CreateGameObject(GameObject* parent, bool createTransform = true);
	inline std::vector<GameObject*> GetGameObjectsList() const 
	{ 
		return root->GetChilds(); 
	}

	inline GameObject* GetRoot() const { return root; }
	inline bool GetGameState() const { return isPlaying; }
	GameObject* GetGoByUuid(double uuid) const;

	void SetMainCamera(CameraComponent* camComponent) { mainCamera = camComponent; }
	void Play();
	void Stop();

	GameObject* Create3DObject(Object3D type, GameObject* parent);

	void MoveGameObjectUp(GameObject* object);
	void MoveGameObjectDown(GameObject* object);
	void ReparentGameObjects(uint uuid, GameObject* go);

	bool LoadScene(const char* name);
	bool SaveScene();

	void AddToQuadtree(GameObject* go);
	void RemoveFromQuadtree(GameObject* go);

	Quadtree& GetQuadtree() { return qTree; }

	CameraComponent* mainCamera;
private:
	GameObject* root;
	bool isPlaying;
	Quadtree qTree;

	EngineTimer time;
};