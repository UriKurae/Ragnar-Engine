#pragma once

#include "Module.h"
#include <vector>

enum class Object3D
{
	CUBE = 0,
	PYRAMIDE,
	SPHERE,
	CYLINDER
};

class GameObject;

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
		return gameObjects; 
	}

	GameObject* Create3DObject(Object3D type);
	//void CreateCube();
	//void CreatePyramide();
	//void CreateSphere();
	//void CreateCylinder();

	void MoveGameObjectUp(GameObject* object);
	void MoveGameObjectDown(GameObject* object);

private:
	std::vector<GameObject*> gameObjects;
};