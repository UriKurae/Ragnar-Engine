#pragma once

#include "Application.h"
#include "MonoManager.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "C_RigidBody.h"
#include "ScriptBindings.h"

#include <metadata\object-forward.h>
#include <metadata\object.h>
#include <metadata/class.h>

#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "Math/float3.h"


void ApplyCentralForce(MonoObject* go, MonoObject* force)
{
	float3 f = app->moduleMono->UnboxVector(force);
	
	RigidBodyComponent* rb = GetComponentMono<RigidBodyComponent*>(go);
	btRigidBody* body = rb->GetBody();
	body->applyCentralForce(f);

	int velMax = 5;
	if (body->getLinearVelocity().norm() > velMax)
		body->setLinearVelocity(body->getLinearVelocity().normalized() * velMax);
}

