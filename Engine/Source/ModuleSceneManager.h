#pragma once
#include "Module.h"

#include <vector>
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

	Scene* GetCurrentScene() const { return currentScene; }

	void NewScene();
	void AddScene(Scene* newScene);

	void ChangeScene(const char* sceneName);

private:

	/*GameState gameState;
	bool frameSkip;*/

	//GameTimer gameTimer;

	Scene* currentScene;

	std::vector<Scene*> scenes;

};