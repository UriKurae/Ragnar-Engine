#include "ModuleScene.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "Physics3D.h"

#include "Primitives.h"
#include "MeshImporter.h"
#include "FileSystem.h"

#include "Resource.h"
#include "ResourceManager.h"
#include "MonoManager.h"
#include "AudioManager.h"

#include "ScriptComponent.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "AudioSourceComponent.h"
#include "AnimationComponent.h"

//Scripting
#include "C_RigidBody.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

#include <stack>
#include "Profiling.h"

ModuleScene::ModuleScene() : sceneDir(""), mainCamera(nullptr), gameState(GameState::NOT_PLAYING), frameSkip(0), resetQuadtree(true), camera(nullptr), player(nullptr)
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

	camera = CreateGameObject(nullptr);
	camera->CreateComponent(ComponentType::CAMERA);
	camera->SetName("Camera");
	camera->CreateComponent(ComponentType::AUDIO_LISTENER);
	//camera->CreateComponent(ComponentType::AUDIO_SOURCE);

	// Register this camera as the default listener.
	AkGameObjectID cameraID = camera->GetUUID();
	AudioManager::Get()->SetDefaultListener(&cameraID, camera->GetComponent<TransformComponent>());
	
	qTree.Create(AABB(float3(-200, -50, -200), float3(200, 50, 200)));
	
	ResourceManager::GetInstance()->ImportResourcesFromLibrary();
	ResourceManager::GetInstance()->ImportAllResources();
	ImportPrimitives();
	ResourceManager::GetInstance()->LoadResource(std::string("Assets/Resources/Street.fbx"));

	player = CreateGameObject(nullptr);
	player->CreateComponent(ComponentType::AUDIO_SOURCE);
	player->SetName("Player");
	player->tag = "Player";
	
	//AkAuxSendValue aEnvs[1];
	//root->GetChilds()[1]->GetChilds()[1]->CreateComponent(ComponentType::AUDIO_REVERB_ZONE);

	//
	//aEnvs[0].listenerID = camera->GetUUID();
	//aEnvs[0].auxBusID = AK::SoundEngine::GetIDFromString(L"ReverbZone");
	//aEnvs[0].fControlValue = 0.0f;
	//
	//if (AK::SoundEngine::SetGameObjectAuxSendValues(camera->GetUUID(), aEnvs, 1) != AK_Success)
	//{
	//	DEBUG_LOG("Couldnt set aux send values");
	//}

	LoadScene("Assets/Scenes/build.ragnar");
	player->GetComponent<AnimationComponent>()->Play("Idle");

	return true;
}

bool ModuleScene::PreUpdate(float dt)
{
	static bool refresh = true;

	if (refresh)
	{
		resetQuadtree = true;
		refresh = false;
	}

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
		//DEBUG_LOG("DELTA TIME GAME %f", gameTimer.GetDeltaTime());
		//DEBUG_LOG("Seconds passed since game startup %d", gameTimer.GetEngineTimeStartup() / 1000);
		frameSkip = false;
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


	///////////////////////
	// Scripting
	Scripting(dt);
	

	AudioManager::Get()->Render();

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
			if (go != app->editor->GetGO()) go->Draw(nullptr);

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
		object->SetParent(parent);
		parent->AddChild(object);

	}
	else
	{
		object->SetParent(root);
		root->AddChild(object);
	}
	
	return object;
}

GameObject* ModuleScene::CreateGameObjectChild(const char* name, GameObject* parent)
{
	GameObject* object = CreateGameObject(parent);
	object->SetName(name);

	return object;
}

GameObject* ModuleScene::CreateGameObjectParent(const char* name, GameObject* child)
{
	GameObject* object = CreateGameObject(child->GetParent());
	object->SetName(name);

	child->GetParent()->RemoveChildren(child->GetParent()->FindChildren(child));
	object->AddChild(child);

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

	gameObj->GetComponent<TransformComponent>()->NewAttachment();
	gameObj->GetComponent<TransformComponent>()->SetAABB();
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
				if (child->GetName() == std::string("Player"))
				{
					player = child;
				}
			}
		}
		for (auto i = referenceMap.begin(); i != referenceMap.end(); ++i)
		{
			// Get the range of the current key
			auto range = referenceMap.equal_range(i->first);

			// Now render out that whole range
			for (auto d = range.first; d != range.second; ++d)
			{
				d->second->fiValue.goValue = GetGoByUuid(d->first);

				if (d->second->fiValue.goValue)
				{
					if (std::find(d->second->fiValue.goValue->csReferences.begin(), d->second->fiValue.goValue->csReferences.end(), d->second) == d->second->fiValue.goValue->csReferences.end())
						d->second->fiValue.goValue->csReferences.push_back(d->second);

					d->second->parentSC->SetField(d->second->field, d->second->fiValue.goValue);
				}
			}
		}
		app->physics->LoadConstraints();
	}
	else
	{
		DEBUG_LOG("Scene couldn't be loaded");
	}

	referenceMap.clear();

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
	//normals.clear();
	//texCoords.clear();
}

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
	app->renderer3D->ClearPointLights();
	app->renderer3D->ClearSpotLights();

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

/////////////////////////

void ModuleScene::Scripting(float dt)
{
	if (gameState == GameState::PLAYING)
	{
		// AUDIO
		if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_DOWN || app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_DOWN || app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_DOWN || app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_DOWN)
		{
			player->GetComponent<AudioSourceComponent>()->PlayClip("footSteps");
		}
		else if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_UP || app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_UP || app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_UP || app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_UP)
		{
			player->GetComponent<AudioSourceComponent>()->StopClip();
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
		{
			player->GetComponent<AudioSourceComponent>()->PlayClip("Shot");
		}
		else if (app->input->GetKey(SDL_SCANCODE_R) == KeyState::KEY_DOWN)
		{
			player->GetComponent<AudioSourceComponent>()->PlayClip("Reload");
		}

		// ANIMATIONS
		if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT ||
			app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT ||
			app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT ||
			app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
		{
			player->GetComponent<AnimationComponent>()->Play("Walk"); //Walk
		}
		else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
			player->GetComponent<AnimationComponent>()->Play("Shoot"); //Shoot

		//ACTIONS
		RigidBodyComponent* playerRB = player->GetComponent<RigidBodyComponent>();
		float playerForce = 100.0f;
		//if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		//{
		//	float force = 10.0f;
		//	GameObject* s = Create3DObject(Object3D::CUBE, nullptr);
		//	s->GetComponent<TransformComponent>()->SetPosition(player->GetComponent<TransformComponent>()->GetPosition());
		//	s->GetComponent<TransformComponent>()->UpdateTransform();
		//	s->GetComponent<TransformComponent>()->ForceUpdateTransform();
		//
		//	RigidBodyComponent* rigidBody;
		//	s->CreateComponent(ComponentType::RIGID_BODY);
		//	rigidBody = s->GetComponent<RigidBodyComponent>();
		//	rigidBody->GetBody()->setIgnoreCollisionCheck(playerRB->GetBody(), true); // Rigid Body of Player
		//	rigidBody->GetBody()->applyCentralImpulse(float3(0,2,0) *force); // Player front normalized
		//}

		//if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		//{
		//	playerRB->GetBody()->applyCentralImpulse(float3(-1, 0, 0) * playerForce * dt);
		//}
		//if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		//{
		//	playerRB->GetBody()->applyCentralImpulse(float3(1, 0, 0) * playerForce * dt);
		//}
		//if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		//{
		//	playerRB->GetBody()->applyCentralImpulse(float3(0, 0, 1) * playerForce * dt);
		//}
		//if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		//{
		//	playerRB->GetBody()->applyCentralImpulse(float3(0, 0, -1) * playerForce * dt);
		//}
	}
}
