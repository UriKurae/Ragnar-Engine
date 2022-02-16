#pragma once

#include "Command.h"

#include "MathGeoLib/src/Math/float3.h"
#include "MathGeoLib/src/Math/Quat.h"

class GameObject;

class MoveGameObjectCommand : public Command
{
public:
	MoveGameObjectCommand();
	MoveGameObjectCommand(GameObject* entity);

	virtual void Execute() override;
	virtual void Undo() override;

	COMMAND_TYPE(MOUSE_MOVE)


private:
	float3 initialPosition = float3::zero;
	Quat   initialRotation = Quat::identity;
	float3 initialScale = float3::zero;

	GameObject* actor;
};