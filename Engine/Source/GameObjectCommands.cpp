#include "GameObjectCommands.h"

#include "GameObject.h"

#include "MathGeoLib/src/Math/float3.h"

MoveGameObjectCommand::MoveGameObjectCommand(GameObject* go) : actor(go)
{
}

MoveGameObjectCommand::MoveGameObjectCommand()
{
}

void MoveGameObjectCommand::Execute()
{
	TransformComponent* tr = actor->GetComponent<TransformComponent>();
	initialPosition = tr->GetPosition();
	initialRotation = tr->GetRotation();
	initialScale = tr->GetScale();
}

void MoveGameObjectCommand::Undo()
{
	TransformComponent* tr = actor->GetComponent<TransformComponent>();
	tr->SetPosition(initialPosition);
	tr->SetRotation(initialRotation);
	tr->UpdateEditorRotation();
	tr->SetScale(initialScale);
	tr->ForceUpdateTransform();
}

void MoveGameObjectCommand::Redo()
{
	TransformComponent* tr = actor->GetComponent<TransformComponent>();
	tr->SetPosition(lastPosition);
	tr->SetRotation(lastRotation);
	tr->SetScale(lastScale);
	tr->ForceUpdateTransform();
}

void MoveGameObjectCommand::GenerateRedo()
{
	TransformComponent* tr = actor->GetComponent<TransformComponent>();
	lastPosition = tr->GetPosition();
	lastRotation = tr->GetRotation();
	lastScale = tr->GetScale();
}
