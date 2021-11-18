#include "ModuleScene.h"

#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "Primitives.h"
#include "ModelImporter.h"
#include "FileSystem.h"

#include <stack>

#include "Profiling.h"

ModuleScene::ModuleScene() : mainCamera(nullptr), isPlaying(false)
{
	root = new GameObject();
	root->SetName("Scene");
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
	
	ModelImporter::ImportModel(std::string("Assets/Resources/BakerHouse.fbx"));
	//app->fs->ImportFiles(std::string("Assets/"));
	ModelImporter::LoadModel(std::string("Assets/Resources/BakerHouse.fbx"));
	//LoadModel::GetInstance()->LoadingModel(std::string("Assets/Resources/soraFbx.fbx"));
	//LoadModel::GetInstance()->LoadingModel(std::string("Assets/Resources/WolfLink.fbx"));

	return true;
}

bool ModuleScene::PreUpdate(float dt)
{
	// TODO: When cleared the idea of the delays, uncomment or delete this.
	//time.Start();

	return true;
}

bool ModuleScene::Update(float dt)
{
	RG_PROFILING_FUNCTION("Updating Scene");

	mainCamera->Update(dt);

	for (int i = 0; i < root->GetChilds().size(); ++i)
		root->GetChilds()[i]->Update(dt);

	return true;
}

bool ModuleScene::PostUpdate()
{
	// TODO: When cleared the idea of the delays, uncomment or delete this.
	//time.FinishUpdate();
	return true;
}

bool ModuleScene::Draw()
{
	RG_PROFILING_FUNCTION("Scene PostUpdate");

	qTree.DebugDraw();

	for (int i = 0; i < root->GetChilds().size(); ++i)
	{
		GameObject* go = root->GetChilds()[i];
		if (go->GetActive())
			go->Draw();
	}

	return true;
}

bool ModuleScene::CleanUp()
{
	RELEASE(root);

	return true;
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
	std::vector<float3> vertices;
	std::vector<float3> normals;
	std::vector<unsigned int> indices;
	std::vector<float2> texCoords;

	std::string path;

	switch (type)
	{
	case Object3D::CUBE:
		object->SetName("Cube");
		RCube::CreateCube(vertices, indices, texCoords);
		path = MESHES_FOLDER + std::string("Cube.rgmesh");
		break;
	case Object3D::PYRAMIDE:
		object->SetName("Pyramide");
		RPyramide::CreatePyramide(vertices, indices, texCoords);
		path = MESHES_FOLDER + std::string("Pyramide.rgmesh");
		break;
	case Object3D::SPHERE:
		object->SetName("Sphere");
		RSphere::CreateSphere(vertices, normals, indices, texCoords);
		path = MESHES_FOLDER + std::string("Sphere.rgmesh");
		break;
	case Object3D::CYLINDER:
		object->SetName("Cylinder");
		RCylinder::CreateCylinder(vertices, normals, indices, texCoords);
		path = MESHES_FOLDER + std::string("Cylinder.rgmesh");
		break;
	}

	if (!vertices.empty())
	{
		MeshComponent* mesh = (MeshComponent*)object->CreateComponent(ComponentType::MESH_RENDERER);;
		mesh->SetMesh(vertices, indices, texCoords, normals, path);
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
	DEBUG_LOG("Loading Scene");

	RELEASE(root);

	char* buffer = nullptr;

	app->fs->Load(name, &buffer);

	if (buffer != nullptr)
	{
		JsonParsing sceneFile = JsonParsing(buffer);

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

	app->editor->SetSelected(nullptr);

	RELEASE_ARRAY(buffer);

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

bool ModuleScene::SaveScene()
{
	DEBUG_LOG("Saving Scene");

	JsonParsing sceneFile;

	sceneFile = sceneFile.SetChild(sceneFile.GetRootValue(), "Scene");
	JSON_Array* array = sceneFile.SetNewJsonArray(sceneFile.GetRootValue(), "Game Objects");
	root->OnSave(sceneFile, array);

	char* buf;
	uint size = sceneFile.Save(&buf);
	
	if (app->fs->Save(SCENES_FOLDER "scene.json", buf, size) > 0)
		DEBUG_LOG("Scene saved succesfully");
	else
		DEBUG_LOG("Scene couldn't be saved");

	RELEASE_ARRAY(buf);

	return true;
}

void ModuleScene::AddToQuadtree(GameObject* go)
{
	qTree.Insert(go);
}

void ModuleScene::RemoveFromQuadtree(GameObject* go)
{
	qTree.Remove(go);
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

	if (app->fs->Save(SCENES_FOLDER "scenePlay.json", buf, size) > 0)
		DEBUG_LOG("Scene saved succesfully");
	else
		DEBUG_LOG("Scene couldn't be saved");

	RELEASE_ARRAY(buf);
	
	isPlaying = true;
}

void ModuleScene::Stop()
{
	LoadScene("Assets/Scenes/scenePlay.json");
	app->fs->RemoveFile("Assets/Scenes/scenePlay.json");
	qTree.Clear();
	qTree.Create(AABB(float3(-200, -50, -200), float3(200, 50, 200)));
	isPlaying = false;
}