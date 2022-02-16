#include "MouseCommands.h"

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
	tr->SetScale(initialScale);
	tr->ForceUpdateTransform();
}
