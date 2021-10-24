#include "ModuleScene.h"

#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "Primitives.h"
#include "Optick/include/optick.h"

#include "LoadModel.h"

#include "mmgr/mmgr.h"

ModuleScene::ModuleScene()
{
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Start()
{
	OPTICK_EVENT("Starting Scene");

	app->camera->position = Vec3(0.0f, 1.0f, -5.0f);
	app->camera->LookAt(Vec3(0, 0, 0));

	LoadModel::GetInstance()->LoadingModel(std::string("Assets/BakerHouse.fbx"));

	return true;
}

bool ModuleScene::Update(float dt)
{
	OPTICK_EVENT("Updating Scene");

	return true;
}

bool ModuleScene::PostUpdate()
{
	OPTICK_EVENT("Scene PostUpdate");

	for (int i = 0; i < gameObjects.size(); ++i)
	{
		gameObjects[i]->Draw();
	}

	return true;
}

bool ModuleScene::CleanUp()
{
	for (int i = 0; i < gameObjects.size(); ++i)
	{
		RELEASE(gameObjects[i]);
	}

	return true;
}

GameObject* ModuleScene::CreateGameObject()
{
	OPTICK_EVENT("Creating Game Object");

	GameObject* object = new GameObject();
	object->CreateComponent(ComponentType::TRANSFORM);
	gameObjects.emplace_back(object);
	
	return object;
}

GameObject* ModuleScene::Create3DObject(Object3D type)
{
	GameObject* object = CreateGameObject();
	MeshComponent* mesh = nullptr;

	switch (type)
	{
	case Object3D::CUBE:
		object->SetName("Cube");
		mesh = new MeshComponent(RCube::GetVertices(), RCube::GetIndices(), RCube::GetTexCoords());
		break;
	case Object3D::PYRAMIDE:
		object->SetName("Pyramide");
		mesh = new MeshComponent(RPyramide::GetVertices(), RPyramide::GetIndices(), RPyramide::GetTexCoords());
		break;
	case Object3D::SPHERE:
		object->SetName("Sphere");
		mesh = new MeshComponent(RSphere::GetVertices(), RSphere::GetIndices(), RSphere::GetTexCoords());
		break;
	case Object3D::CYLINDER:
		object->SetName("Cylinder");
		mesh = new MeshComponent(RCylinder::GetVertices(), RCylinder::GetIndices(), RCylinder::GetTexCoords());
		break;
	}

	mesh->SetOwner(object);
	mesh->SetTransform(object->GetComponent<TransformComponent>());
	Checker::CheckerImage checker = Checker::CreateChecker();
	MaterialComponent* material = new MaterialComponent(checker.id, checker.width, checker.height, checker.checkerImage);
	mesh->SetMaterial(material);
	object->AddComponent(mesh);
	object->AddComponent(material);

	return object;
}

void ModuleScene::MoveGameObjectUp(GameObject* object)
{
	if (object == gameObjects[0]) return;

	int size = gameObjects.size();
	for (int i = 0; i < size; ++i)
	{
		if (gameObjects[i] == object)
		{
			GameObject* aux = gameObjects[i];

			gameObjects[i] = gameObjects[i - 1];
			gameObjects[i - 1] = aux;
			break;
		}
	}
}

void ModuleScene::MoveGameObjectDown(GameObject* object)
{
	int size = gameObjects.size() - 1;
	if (object == gameObjects[size]) return;

	for (int i = size; i >= 0; --i)
	{
		if (gameObjects[i] == object)
		{
			GameObject* aux = gameObjects[i];

			gameObjects[i] = gameObjects[i + 1];
			gameObjects[i + 1] = aux;
			break;
		}
	}
}