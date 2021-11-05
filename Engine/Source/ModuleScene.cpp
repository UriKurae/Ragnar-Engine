#include "ModuleScene.h"

#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "Primitives.h"
#include "LoadModel.h"
#include "FileSystem.h"

#include "Profiling.h"

ModuleScene::ModuleScene()
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

GameObject* ModuleScene::CreateGameObject(GameObject* parent)
{
	RG_PROFILING_FUNCTION("Creating Game Object");

	GameObject* object = new GameObject();
	object->CreateComponent(ComponentType::TRANSFORM);
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

	MeshComponent* mesh = nullptr;

	switch (type)
	{
	case Object3D::CUBE:
		object->SetName("Cube");
		RCube::CreateCube(vertices, indices, texCoords);
		break;
	case Object3D::PYRAMIDE:
		object->SetName("Pyramide");
		RPyramide::CreatePyramide(vertices, indices, texCoords);
		break;
	case Object3D::SPHERE:
		object->SetName("Sphere");
		RSphere::CreateSphere(vertices, normals, indices, texCoords);
		break;
	case Object3D::CYLINDER:
		object->SetName("Cylinder");
		RCylinder::CreateCylinder(vertices, normals, indices, texCoords);
		break;
	}

	if (!vertices.empty())
	{
		mesh = new MeshComponent(object, object->GetComponent<TransformComponent>());
		mesh->SetMesh(vertices, indices, texCoords, normals);
		mesh->SetOwner(object);
		mesh->SetTransform(object->GetComponent<TransformComponent>());
		object->AddComponent(mesh);
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

bool ModuleScene::LoadScene()
{
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

	RELEASE_ARRAY(buf);

	return true;
}