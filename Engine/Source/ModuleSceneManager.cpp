#include <Globals.h>
#include "Application.h"

#include "ModuleRenderer3D.h"
#include "ModuleSceneManager.h"
#include "Scene.h"

#include "ResourceManager.h"
#include "Resource.h"

#include "FileSystem.h"

#include "MeshImporter.h"
#include "Bone.h"
#include "Primitives.h"

#include "MonoManager.h"
#include "ScriptComponent.h"

#include "Profiling.h"

ModuleSceneManager::ModuleSceneManager(bool startEnabled) : changeScene(false), index(0), gameState(GameState::NOT_PLAYING), Module(startEnabled)
{
	uint uid = ResourceManager::GetInstance()->CreateResource(ResourceType::SCENE, std::string(""), std::string(""));
	currentScene = std::static_pointer_cast<Scene>(ResourceManager::GetInstance()->GetResource(uid));
	AddScene(currentScene);
}

ModuleSceneManager::~ModuleSceneManager()
{
	currentScene = nullptr;
	//for (int i = 0; i < scenes.size(); ++i)
	//{
	//	RELEASE(scenes[i]);
	//}
}

bool ModuleSceneManager::Start()
{
	ResourceManager::GetInstance()->ImportResourcesFromLibrary();
	ResourceManager::GetInstance()->ImportAllResources();
	ImportPrimitives();

	currentScene->Start();

	referenceMap.clear();

	return true;
}

bool ModuleSceneManager::PreUpdate(float dt)
{
	currentScene->PreUpdate(gameTimer.GetDeltaTime());

	if (gameState == GameState::PLAYING) gameTimer.Start();

	return true;
}

bool ModuleSceneManager::Update(float dt)
{
	if (changeScene)
	{
		currentScene->UnLoad();
		currentScene = scenes[index];
		currentScene->Load();
		newSceneLoaded = true;
		changeScene = false;
	}

	currentScene->Update(gameTimer.GetDeltaTime());
	
	return true;
}

bool ModuleSceneManager::PostUpdate()
{
	if (gameState == GameState::PLAYING) gameTimer.FinishUpdate();

	currentScene->PostUpdate();

	return true;
}

bool ModuleSceneManager::Draw()
{
	currentScene->Draw();

	return true;
}

bool ModuleSceneManager::CleanUp()
{
	for (int i = 0; i < scenes.size(); ++i)
	{
		scenes[i]->CleanUp();
	}

	return true;
}

void ModuleSceneManager::ImportPrimitives()
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::map<std::string, BoneInfo> bonesUid;
	//std::vector<float3> normals;
	//std::vector<float2> texCoords;

	RCube::CreateCube(vertices, indices);
	std::string library;
	ResourceManager::GetInstance()->CreateResource(ResourceType::MESH, std::string("Settings/EngineResources/__Cube.mesh"), library);
	MeshImporter::SaveMesh(library, vertices, indices, bonesUid);

	vertices.clear();
	indices.clear();
	//normals.clear();
	//texCoords.clear();
	library.clear();

	RPyramide::CreatePyramide(vertices, indices);
	ResourceManager::GetInstance()->CreateResource(ResourceType::MESH, std::string("Settings/EngineResources/__Pyramide.mesh"), library);
	MeshImporter::SaveMesh(library, vertices, indices, bonesUid);

	vertices.clear();
	indices.clear();
	//normals.clear();
	//texCoords.clear();
	library.clear();

	RSphere::CreateSphere(vertices, indices);
	ResourceManager::GetInstance()->CreateResource(ResourceType::MESH, std::string("Settings/EngineResources/__Sphere.mesh"), library);
	MeshImporter::SaveMesh(library, vertices, indices, bonesUid);

	vertices.clear();
	indices.clear();
	//normals.clear();
	//texCoords.clear();
	library.clear();

	RCylinder::CreateCylinder(vertices, indices);
	ResourceManager::GetInstance()->CreateResource(ResourceType::MESH, std::string("Settings/EngineResources/__Cylinder.mesh"), library);
	MeshImporter::SaveMesh(library, vertices, indices, bonesUid);

	vertices.clear();
	indices.clear();
}

void ModuleSceneManager::NewScene()
{
	currentScene->NewScene();
}

void ModuleSceneManager::AddScene(std::shared_ptr<Scene> newScene)
{
	scenes.push_back(newScene);
}

void ModuleSceneManager::DeleteScene(std::shared_ptr<Scene> scene)
{
	for (int i = 0; i < scenes.size(); ++i)
	{
		if (scenes[i] == scene)
		{
			scenes.erase(scenes.begin() + i);
			break;
		}
	}
}

void ModuleSceneManager::ChangeScene(const char* sceneName)
{
	if (currentScene->GetAssetsPath() == "")
	{
		ResourceManager::GetInstance()->DeleteResource(currentScene->GetUID());
	}
	currentScene = std::static_pointer_cast<Scene>(ResourceManager::GetInstance()->LoadResource(std::string(sceneName)));
}

void ModuleSceneManager::NextScene()
{
	if (index == scenes.size() - 1) index = 0;
	else ++index;
	changeScene = true;
}

void ModuleSceneManager::NextScene(const char* name)
{
	for (int i = 0; i < scenes.size(); ++i)
	{
		if (scenes[i]->GetName() == name)
		{
			index = i;
			changeScene = true;
			break;
		}
	}
}

void ModuleSceneManager::Play()
{
	DEBUG_LOG("Saving Scene");

	JsonParsing sceneFile;

	sceneFile = sceneFile.SetChild(sceneFile.GetRootValue(), "Scene");
	JSON_Array* array = sceneFile.SetNewJsonArray(sceneFile.GetRootValue(), "Game Objects");
	currentScene->GetRoot()->OnSave(sceneFile, array);

	char* buf;
	uint size = sceneFile.Save(&buf);

	if (app->fs->Save(SCENES_FOLDER "scenePlay.ragnar", buf, size) > 0)
		DEBUG_LOG("Scene saved succesfully");
	else
		DEBUG_LOG("Scene couldn't be saved");

	RELEASE_ARRAY(buf);

	gameState = GameState::PLAYING;
	gameTimer.ResetTimer();

	currentScene = scenes[index];
}

void ModuleSceneManager::Stop()
{
	app->renderer3D->ClearPointLights();
	app->renderer3D->ClearSpotLights();

	currentScene->LoadScene("Assets/Scenes/scenePlay.ragnar");
	app->fs->RemoveFile("Assets/Scenes/scenePlay.ragnar");
	currentScene->GetQuadtree().Clear();
	currentScene->GetQuadtree().Create(AABB(float3(-200, -50, -200), float3(200, 50, 200)));
	gameState = GameState::NOT_PLAYING;
}

void ModuleSceneManager::Pause()
{
	gameTimer.SetDesiredDeltaTime(0.0f);
	gameState = GameState::PAUSE;
}

void ModuleSceneManager::Resume()
{
	gameTimer.SetDesiredDeltaTime(0.016f);
	gameState = GameState::PLAYING;
}