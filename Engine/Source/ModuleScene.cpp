#include "ModuleScene.h"

#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "Primitives.h"
#include "LoadModel.h"
#include "FileSystem.h"
#include "TextureLoader.h"

#include "Profiling.h"

ModuleScene::ModuleScene() : mainCamera(nullptr)
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
	
	LoadModel::GetInstance()->ImportModel(std::string("Assets/Resources/BakerHouse.fbx"));
	LoadModel::GetInstance()->LoadingModel(std::string("Assets/Resources/BakerHouse.fbx"));
	//LoadModel::GetInstance()->LoadingModel(std::string("Assets/Resources/soraFbx.fbx"));
	//LoadModel::GetInstance()->LoadingModel(std::string("Assets/Resources/WolfLink.fbx"));

	return true;
}

bool ModuleScene::Update(float dt)
{
	RG_PROFILING_FUNCTION("Updating Scene");

	return true;
}

bool ModuleScene::Draw()
{
	RG_PROFILING_FUNCTION("Scene PostUpdate");

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
			else if (go.GetJsonNumber("Parent UUID") == root->GetUUID())
			{
				GameObject* object = CreateGameObject(root, false);
				object->OnLoad(go);
			}
			else
			{
				for (int i = 0; i < root->GetChilds().size(); ++i)
				{
					if (go.GetJsonNumber("Parent UUID") == root->GetChilds()[i]->GetUUID())
					{
						GameObject* object = CreateGameObject(root->GetChilds()[i], false);
						object->OnLoad(go);
					}
				}
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