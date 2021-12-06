#include "ModuleScene.h"

#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "Primitives.h"
#include "MeshImporter.h"
#include "FileSystem.h"
#include "Resource.h"
#include "ResourceManager.h"

#include <stack>

#include "Profiling.h"

ModuleScene::ModuleScene() : sceneDir(""), mainCamera(nullptr), gameState(GameState::NOT_PLAYING), frameSkip(0), resetQuadtree(true), goToRecalculate(nullptr)
{
	root = new GameObject();
	root->SetName("Untitled");
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Start()
{
	RG_PROFILING_FUNCTION("Starting Scene");

	GameObject* camera = CreateGameObject(nullptr);
	camera->CreateComponent(ComponentType::CAMERA);
	camera->SetName("Camera");
	
	qTree.Create(AABB(float3(-200, -50, -200), float3(200, 50, 200)));
	
	ResourceManager::GetInstance()->ImportResourcesFromLibrary();
	ResourceManager::GetInstance()->ImportAllResources();
	ImportPrimitives();
	ResourceManager::GetInstance()->LoadResource(std::string("Assets/Resources/Street.fbx"));

	return true;
}

bool ModuleScene::PreUpdate(float dt)
{
	if (gameState == GameState::PLAYING) gameTimer.Start();

	return true;
}

bool ModuleScene::Update(float dt)
{
	RG_PROFILING_FUNCTION("Updating Scene");

	if (mainCamera != nullptr) mainCamera->Update(gameTimer.GetDeltaTime());

	for (int i = 0; i < root->GetChilds().size(); ++i)
		root->GetChilds()[i]->Update(gameTimer.GetDeltaTime());
	
	if (frameSkip || gameState == GameState::PLAYING)
	{
		DEBUG_LOG("DELTA TIME GAME %f", gameTimer.GetDeltaTime());
		DEBUG_LOG("Seconds passed since game startup %d", gameTimer.GetEngineTimeStartup() / 1000);
		frameSkip = false;
	}

	if (goToRecalculate && goToRecalculate->GetParent() != root)
	{
		std::stack<GameObject*> objects;
		objects.push(goToRecalculate->GetParent());
		while (!objects.empty())
		{
			GameObject* parent = objects.top();
			objects.pop();

			parent->ClearAABB();
			parent->SetNewAABB();

			if (parent->GetParent() != root) objects.push(parent->GetParent());
		}

		goToRecalculate = nullptr;
	}

	if (resetQuadtree)
	{
		qTree.Clear();
		qTree.Create(AABB(float3(-200, -50, -200), float3(200, 50, 200)));
		std::stack<GameObject*> objects;

		for (int i = 0; i < root->GetChilds().size(); ++i)
			objects.push(root->GetChilds()[i]);

		while (!objects.empty())
		{
			GameObject* go = objects.top();
			objects.pop();

			qTree.Insert(go);

			for (int i = 0; i < go->GetChilds().size(); ++i)
				objects.push(go->GetChilds()[i]);
		}

		resetQuadtree = false;
	}

	return true;
}

bool ModuleScene::PostUpdate()
{
	if (gameState == GameState::PLAYING) gameTimer.FinishUpdate();

	return true;
}

bool ModuleScene::Draw()
{
	RG_PROFILING_FUNCTION("Scene PostUpdate");

	qTree.DebugDraw();

	std::stack<GameObject*> stack;

	for (int i = 0; i < root->GetChilds().size(); ++i)
		stack.push(root->GetChilds()[i]);

	while (!stack.empty())
	{
		GameObject* go = stack.top();
		stack.pop();

		if (go->GetActive())
		{
			go->Draw();

			for (int i = 0; i < go->GetChilds().size(); ++i)
				stack.push(go->GetChilds()[i]);
		}
	}
	//for (int i = 0; i < root->GetChilds().size(); ++i)
	//{
	//	GameObject* go = root->GetChilds()[i];
	//	if (go->GetActive())
	//		go->Draw();
	//}

	return true;
}

bool ModuleScene::CleanUp()
{
	RELEASE(root);

	return true;
}

void ModuleScene::NewScene()
{
	RELEASE(root);

	sceneDir.clear();

	root = new GameObject();
	root->SetName("Untitled");

	GameObject* camera = CreateGameObject(nullptr);
	camera->CreateComponent(ComponentType::CAMERA);
	camera->SetName("Camera");

	qTree.Create(AABB(float3(-200, -50, -200), float3(200, 50, 200)));

	app->editor->SetGO(nullptr);
}

GameObject* ModuleScene::CreateGameObject(GameObject* parent, bool createTransform)
{
	RG_PROFILING_FUNCTION("Creating Game Object");

	GameObject* object = new GameObject();
	if (createTransform) object->CreateComponent(ComponentType::TRANSFORM);
	if (parent != nullptr)
	{
		parent->AddChild(object);
		object->SetParent(parent);
	}
	else
	{
		root->AddChild(object);
		object->SetParent(root);
	}
	
	return object;
}

GameObject* ModuleScene::Create3DObject(Object3D type, GameObject* parent)
{
	GameObject* object = CreateGameObject(parent);
	std::string path;

	switch (type)
	{
	case Object3D::CUBE:
		object->SetName("Cube");
		path = "Settings/EngineResources/__Cube.mesh";
		break;
	case Object3D::PYRAMIDE:
		object->SetName("Pyramide");
		path = "Settings/EngineResources/__Pyramide.mesh";
		break;
	case Object3D::SPHERE:
		object->SetName("Sphere");
		path = "Settings/EngineResources/__Sphere.mesh";
		break;
	case Object3D::CYLINDER:
		object->SetName("Cylinder");
		path = "Settings/EngineResources/__Cylinder.mesh";
		break;
	}

	if (!path.empty())
	{
		MeshComponent* mesh = (MeshComponent*)object->CreateComponent(ComponentType::MESH_RENDERER);
		mesh->SetMesh(ResourceManager::GetInstance()->LoadResource(path));
	}

	return object;
}

void ModuleScene::MoveGameObjectUp(GameObject* object)
{
	if (object == root->GetChilds()[0]) return;

	int size = root->GetChilds().size();
	for (int i = 0; i < size; ++i)
	{
		if (root->GetChilds()[i] == object)
		{
			GameObject* aux = root->GetChilds()[i];

			root->GetChilds()[i] = root->GetChilds()[i - 1];
			root->GetChilds()[i - 1] = aux;
			break;
		}
	}
}

void ModuleScene::MoveGameObjectDown(GameObject* object)
{
	int size = root->GetChilds().size() - 1;
	if (object == root->GetChilds()[size]) return;

	for (int i = size; i >= 0; --i)
	{
		if (root->GetChilds()[i] == object)
		{
			GameObject* aux = root->GetChilds()[i];

			root->GetChilds()[i] = root->GetChilds()[i + 1];
			root->GetChilds()[i + 1] = aux;
			break;
		}
	}
}

void ModuleScene::ReparentGameObjects(uint uuid, GameObject* go)
{
	GameObject* gameObj = GetGoByUuid(uuid);
	GameObject* parentObj = gameObj->GetParent();

	parentObj->RemoveChild(gameObj);
	gameObj->SetParent(go);
	go->AddChild(gameObj);
}

bool ModuleScene::LoadScene(const char* name)
{
	RG_PROFILING_FUNCTION("Loading Scene");

	DEBUG_LOG("Loading Scene");

	RELEASE(root);

	//char* buffer = nullptr;

	//app->fs->Load(name, &buffer);
	sceneDir = name;

	JsonParsing sceneFile = JsonParsing();

	if (sceneFile.ParseFile(name) > 0)
	{
		JSON_Array* jsonArray = sceneFile.GetJsonArray(sceneFile.ValueToObject(sceneFile.GetRootValue()), "Game Objects");
		
		size_t size = sceneFile.GetJsonArrayCount(jsonArray);
		for (int i = 0; i < size; ++i)
		{
			JsonParsing go = sceneFile.GetJsonArrayValue(jsonArray, i);
			if (go.GetJsonNumber("Parent UUID") == 0)
			{
				root = new GameObject();
				root->OnLoad(go);
			}
			else
			{
				GameObject* parent = GetGoByUuid(go.GetJsonNumber("Parent UUID"));
				GameObject* child = CreateGameObject(parent, false);
				child->OnLoad(go);
			}
		}
	}
	else
	{
		DEBUG_LOG("Scene couldn't be loaded");
	}

	// TODO: Check this because it can be much cleaner
	qTree.Clear();
	qTree.Create(AABB(float3(-200, -50, -200), float3(200, 50, 200)));
	app->editor->SetGO(nullptr);

	return true;
}

GameObject* ModuleScene::GetGoByUuid(double uuid) const
{
	std::stack<GameObject*> goStack;
	goStack.push(root);

	while (!goStack.empty())
	{
		GameObject* go = goStack.top();
		goStack.pop();
		if (go->GetUUID() == uuid)	return go;
		else
		{
			for (int i = 0; i < go->GetChilds().size(); ++i)
			{
				goStack.push(go->GetChilds()[i]);
			}
		}
	}

	return nullptr;
}

bool ModuleScene::SaveScene(const char* name)
{
	DEBUG_LOG("Saving Scene");

	sceneDir = name;

	std::string rootName = name;
	app->fs->GetFilenameWithoutExtension(rootName);
	root->SetName(rootName.c_str());

	JsonParsing sceneFile;

	sceneFile = sceneFile.SetChild(sceneFile.GetRootValue(), "Scene");
	JSON_Array* array = sceneFile.SetNewJsonArray(sceneFile.GetRootValue(), "Game Objects");
	root->OnSave(sceneFile, array);

	uint size = sceneFile.SaveFile(name);
	
	if (size > 0)
		DEBUG_LOG("Scene saved succesfully");
	else
		DEBUG_LOG("Scene couldn't be saved");

	return true;
}

void ModuleScene::DuplicateGO(GameObject* go, GameObject* parent)
{
	GameObject* gameObject = new GameObject();
	gameObject->SetName(go->GetName());

	gameObject->SetParent(parent);
	parent->AddChild(gameObject);

	for (int i = 0; i < go->GetComponents().size(); ++i)
	{
		gameObject->CopyComponent(go->GetComponents()[i]);
	}

	for (int i = 0; i < go->GetChilds().size(); ++i)
	{
		DuplicateGO(go->GetChilds()[i], gameObject);
	}
	//gameObject->SetAABB(go->GetAABB());
}

void ModuleScene::ImportPrimitives()
{
	std::vector<float3> vertices;
	std::vector<unsigned int> indices;
	std::vector<float3> normals;
	std::vector<float2> texCoords;

	RCube::CreateCube(vertices, indices, texCoords);
	std::string library;
	ResourceManager::GetInstance()->CreateResource(ResourceType::MESH, std::string("Settings/EngineResources/__Cube.mesh"), library);
	MeshImporter::SaveMesh(library, vertices, indices, normals, texCoords);

	vertices.clear();
	indices.clear();
	normals.clear();
	texCoords.clear();
	library.clear();

	RPyramide::CreatePyramide(vertices, indices, texCoords);
	ResourceManager::GetInstance()->CreateResource(ResourceType::MESH, std::string("Settings/EngineResources/__Pyramide.mesh"), library);
	MeshImporter::SaveMesh(library, vertices, indices, normals, texCoords);

	vertices.clear();
	indices.clear();
	normals.clear();
	texCoords.clear();
	library.clear();

	RSphere::CreateSphere(vertices, normals, indices, texCoords);
	ResourceManager::GetInstance()->CreateResource(ResourceType::MESH, std::string("Settings/EngineResources/__Sphere.mesh"), library);
	MeshImporter::SaveMesh(library, vertices, indices, normals, texCoords);

	vertices.clear();
	indices.clear();
	normals.clear();
	texCoords.clear();
	library.clear();

	RCylinder::CreateCylinder(vertices, normals, indices, texCoords);
	ResourceManager::GetInstance()->CreateResource(ResourceType::MESH, std::string("Settings/EngineResources/__Cylinder.mesh"), library);
	MeshImporter::SaveMesh(library, vertices, indices, normals, texCoords);

	vertices.clear();
	indices.clear();
	normals.clear();
	texCoords.clear();
}

//void ModuleScene::AddToQuadtree(GameObject* go)
//{
//	qTree.Insert(go);
//}
//
//void ModuleScene::RemoveFromQuadtree(GameObject* go)
//{
//	qTree.Remove(go);
//}

void ModuleScene::Play()
{
	DEBUG_LOG("Saving Scene");

	JsonParsing sceneFile;

	sceneFile = sceneFile.SetChild(sceneFile.GetRootValue(), "Scene");
	JSON_Array* array = sceneFile.SetNewJsonArray(sceneFile.GetRootValue(), "Game Objects");
	root->OnSave(sceneFile, array);

	char* buf;
	uint size = sceneFile.Save(&buf);

	if (app->fs->Save(SCENES_FOLDER "scenePlay.ragnar", buf, size) > 0)
		DEBUG_LOG("Scene saved succesfully");
	else
		DEBUG_LOG("Scene couldn't be saved");

	RELEASE_ARRAY(buf);
	
	gameState = GameState::PLAYING;
	gameTimer.ResetTimer();
}

void ModuleScene::Stop()
{
	LoadScene("Assets/Scenes/scenePlay.ragnar");
	app->fs->RemoveFile("Assets/Scenes/scenePlay.ragnar");
	qTree.Clear();
	qTree.Create(AABB(float3(-200, -50, -200), float3(200, 50, 200)));
	gameState = GameState::NOT_PLAYING;
}

void ModuleScene::Pause()
{
	gameTimer.SetDesiredDeltaTime(0.0f);
	gameState = GameState::PAUSE;
}

void ModuleScene::Resume()
{
	gameTimer.SetDesiredDeltaTime(0.016f);
	gameState = GameState::PLAYING;
}