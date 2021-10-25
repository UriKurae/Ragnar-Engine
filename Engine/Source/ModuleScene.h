#pragma once

#include "Module.h"
#include "GameObject.h"
#include <vector>

enum class Object3D
{
	CUBE = 0,
	PYRAMIDE,
	SPHERE,
	CYLINDER
};

class ModuleScene : public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Start() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	GameObject* CreateGameObject();
	inline std::vector<GameObject*> GetGameObjectsList() const 
	{ 
		return root->GetChilds(); 
	}

	inline GameObject* GetRoot() const { return root; }

	GameObject* Create3DObject(Object3D type);

	void MoveGameObjectUp(GameObject* object);
	void MoveGameObjectDown(GameObject* object);

private:
	GameObject* root;
};