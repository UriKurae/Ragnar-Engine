#include <Globals.h>

#include "ModuleSceneManager.h"
#include "Scene.h"

#include "ResourceManager.h"
#include "Resource.h"

#include "MeshImporter.h"
#include "Bone.h"
#include "Primitives.h"

#include "Profiling.h"

ModuleSceneManager::ModuleSceneManager(bool startEnabled) : Module(startEnabled)
{
	currentScene = new Scene();
	AddScene(currentScene);
}

ModuleSceneManager::~ModuleSceneManager()
{
	currentScene = nullptr;
	for (int i = 0; i < scenes.size(); ++i)
	{
		RELEASE(scenes[i]);
	}
}

bool ModuleSceneManager::Start()
{
	ResourceManager::GetInstance()->ImportResourcesFromLibrary();
	ResourceManager::GetInstance()->ImportAllResources();
	ImportPrimitives();

	currentScene->Start();

	return true;
}

bool ModuleSceneManager::PreUpdate(float dt)
{
	for (int i = 0; i < scenes.size(); ++i)
	{
		scenes[i]->PreUpdate(dt);
	}
	return true;
}

bool ModuleSceneManager::Update(float dt)
{
	for (int i = 0; i < scenes.size(); ++i)
	{
		scenes[i]->Update(dt);
	}
	return true;
}

bool ModuleSceneManager::PostUpdate()
{
	for (int i = 0; i < scenes.size(); ++i)
	{
		scenes[i]->PostUpdate();
	}
	return true;
}

bool ModuleSceneManager::Draw()
{
	for (int i = 0; i < scenes.size(); ++i)
	{
		scenes[i]->Draw();
	}
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

void ModuleSceneManager::AddScene(Scene* newScene)
{
	scenes.push_back(newScene);
}

void ModuleSceneManager::ChangeScene(const char* sceneName)
{
	currentScene->LoadScene(sceneName);
}