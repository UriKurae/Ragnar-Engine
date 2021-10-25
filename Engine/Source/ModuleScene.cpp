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
	root = new GameObject();
	root->SetName("Scene");
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

	for (int i = 0; i < root->GetChilds().size(); ++i)
	{
		root->GetChilds()[i]->Draw();
	}

	return true;
}

bool ModuleScene::CleanUp()
{
	RELEASE(root);

	return true;
}

GameObject* ModuleScene::CreateGameObject()
{
	OPTICK_EVENT("Creating Game Object");

	GameObject* object = new GameObject();
	object->CreateComponent(ComponentType::TRANSFORM);
	//gameObjects.emplace_back(object);
	root->AddChild(object);
	
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