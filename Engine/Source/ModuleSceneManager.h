#pragma once
#include "Module.h"

#include <vector>
#include <memory>
#include <map>

class Scene;


//struct SerializedField;
//class btRigidBody; // Scripting


// Already in Scene.h
//enum class Object3D
//{
//	CUBE = 0,
//	PYRAMIDE,
//	SPHERE,
//	CYLINDER
//};

//enum class GameState
//{
//	NOT_PLAYING = 0,
//	PLAYING,
//	PAUSE
//};
class CameraComponent;

class ModuleSceneManager : public Module
{
public:
	ModuleSceneManager(bool startEnabled = true);
	~ModuleSceneManager();

	bool Start() override;
	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool Draw();
	bool CleanUp() override;

	void ImportPrimitives();

	Scene* GetCurrentScene() const { return currentScene.get(); }

	void NewScene();
	void AddScene(std::shared_ptr<Scene> newScene);

	void ChangeScene(const char* sceneName);

	void NextScene();
	void NextScene(const char* name);

private:
	int index;
	/*GameState gameState;
	bool frameSkip;*/

	//GameTimer gameTimer;

	std::shared_ptr<Scene> currentScene;

	std::vector<std::shared_ptr<Scene>> scenes;
};