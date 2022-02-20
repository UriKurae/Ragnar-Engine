#pragma once

#include "Command.h"

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"

class GameObject;

class MoveGameObjectCommand : public Command
{
public:
	MoveGameObjectCommand();
	MoveGameObjectCommand(GameObject* entity);

	virtual void Execute() override;
	virtual void Undo() override;
	virtual void Redo() override;

	virtual void GenerateRedo() override;

	COMMAND_TYPE(MOUSE_MOVE)

private:
	float3 initialPosition = float3::zero;
	Quat   initialRotation = Quat::identity;
	float3 initialScale = float3::zero;

	float3 lastPosition = float3::zero;
	Quat   lastRotation = Quat::identity;
	float3 lastScale = float3::zero;


	GameObject* actor;
};