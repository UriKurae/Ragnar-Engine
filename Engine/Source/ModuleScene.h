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
	bool Draw();
	bool CleanUp() override;

	GameObject* CreateGameObject(GameObject* parent, bool createTransform = true);
	inline std::vector<GameObject*> GetGameObjectsList() const 
	{ 
		return root->GetChilds(); 
	}

	inline GameObject* GetRoot() const { return root; }

	GameObject* Create3DObject(Object3D type, GameObject* parent);

	void MoveGameObjectUp(GameObject* object);
	void MoveGameObjectDown(GameObject* object);

	bool LoadScene(const char* name);
	bool SaveScene();

private:
	GameObject* root;
};