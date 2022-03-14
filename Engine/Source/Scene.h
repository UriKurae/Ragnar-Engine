#pragma once

#include "GameObject.h"
#include "Quadtree.h"
#include "GameTimer.h"

#include <vector>
#include <map>

struct SerializedField;
class btRigidBody; // Scripting

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

class Scene
{
public:
	Scene();
	~Scene();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate();
	bool Draw();
	bool CleanUp();

	void NewScene();

	GameObject* CreateGameObject(GameObject* parent, bool createTransform = true);
	GameObject* CreateGameObjectChild(const char* name, GameObject* parent);
	GameObject* CreateGameObjectParent(const char* name, GameObject* child);
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

	void DuplicateGO(GameObject* go, GameObject* parent);

	inline void ResetQuadtree() { resetQuadtree = true; }

	inline const std::string& SceneDirectory() const { return sceneDir; }

	Quadtree& GetQuadtree() { return qTree; }
	void SetGameDeltaTime(float deltaTime) { gameTimer.SetDesiredDeltaTime(deltaTime); }
	inline GameObject* GetPlayer() { return player; };

	// Scripting
	void Scripting(float dt);
	void SetVelocityPlayer(btRigidBody* playerRB, math::float3& vel);

	CameraComponent* mainCamera;
	GameObject* camera;

	std::multimap<uint, SerializedField*> referenceMap;

private:
	GameObject* root;
	Quadtree qTree;
	GameState gameState;
	bool frameSkip;

	GameObject* player;
	

	bool resetQuadtree;

	GameTimer gameTimer;

	std::string sceneDir;
};