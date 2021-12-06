#pragma once

#include "Module.h"
#include "GameObject.h"
#include "Quadtree.h"
#include "GameTimer.h"
#include <vector>

enum class Object3D
{
	CUBE = 0,
	PYRAMIDE,
	SPHERE,
	CYLINDER
};

enum class GameState
{
	NOT_PLAYING = 0,
	PLAYING,
	PAUSE
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

	void NewScene();

	GameObject* CreateGameObject(GameObject* parent, bool createTransform = true);
	inline std::vector<GameObject*> GetGameObjectsList() const 
	{ 
		return root->GetChilds(); 
	}

	inline GameObject* GetRoot() const { return root; }
	inline GameState GetGameState() const { return gameState; }
	GameObject* GetGoByUuid(double uuid) const;

	void SetMainCamera(CameraComponent* camComponent) { mainCamera = camComponent; }
	void Play();
	void Stop();
	void Pause();
	void Resume();
	inline void NextFrame() { frameSkip = true; }

	GameObject* Create3DObject(Object3D type, GameObject* parent);

	void MoveGameObjectUp(GameObject* object);
	void MoveGameObjectDown(GameObject* object);
	void ReparentGameObjects(uint uuid, GameObject* go);

	bool LoadScene(const char* name);
	bool SaveScene(const char* name);

	inline void RecalculateAABB(GameObject* go) { goToRecalculate = go; }

	void DuplicateGO(GameObject* go, GameObject* parent);

	//void AddToQuadtree(GameObject* go);
	//void RemoveFromQuadtree(GameObject* go);
	void ImportPrimitives();

	inline void ResetQuadtree() { resetQuadtree = true; }

	inline const std::string& SceneDirectory() const { return sceneDir; }

	inline const GameObject* GetRecalculateGO() const { return goToRecalculate; }

	Quadtree& GetQuadtree() { return qTree; }
	void SetGameDeltaTime(float deltaTime) { gameTimer.SetDesiredDeltaTime(deltaTime); }

	CameraComponent* mainCamera;
private:
	GameObject* root;
	Quadtree qTree;
	GameState gameState;
	bool frameSkip;

	bool resetQuadtree;

	GameTimer gameTimer;
	GameObject* goToRecalculate;

	std::string sceneDir;
};