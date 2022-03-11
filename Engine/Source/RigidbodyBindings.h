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
	char* nameGo = mono_string_to_utf8(mono_object_to_string(go, 0));
	char* nameForce = mono_string_to_utf8(mono_object_to_string(force, 0));


	float3 f = app->moduleMono->UnboxVector(force);
	
	RigidBodyComponent* rb = GetComponentMono<RigidBodyComponent*>(go);
	btRigidBody* body = rb->GetBody();
	body->applyCentralForce(f);

	int velMax = 5;
	if (body->getLinearVelocity().norm() > velMax)
		body->setLinearVelocity(body->getLinearVelocity().normalized() * velMax);

	//TransformComponent* tr = rb->owner->GetComponent<TransformComponent>();
	//tr->ForceUpdateTransform();

}

void SetLinearVelocity(MonoObject* go, MonoObject* velocity)
{
	char* nameGo = mono_string_to_utf8(mono_object_to_string(go, 0));
	char* name = mono_string_to_utf8(mono_object_to_string(velocity, 0));

	float3 vel = app->moduleMono->UnboxVector(velocity);
	RigidBodyComponent* rb = GetComponentMono<RigidBodyComponent*>(go);
	rb->GetBody()->setLinearVelocity({ vel.x, vel.y, vel.z });
}

void SetIgnoreCollision(MonoObject* go, bool value)
{
	char* nameGo = mono_string_to_utf8(mono_object_to_string(go, 0));
	//std::string name = nameGo;
	//GameObject* gameObject = 0;
	//if (name.find("Component") != -1)
	//{
	//	gameObject = GetGameObjectMono(go);
	//	RigidBodyComponent* rb = gameObject->GetComponent<RigidBodyComponent>();
	//	rb->GetBody()->setIgnoreCollisionCheck(rb->GetBody(), value);
	//}
	//else
	//{
		RigidBodyComponent* rb = GetComponentMono<RigidBodyComponent*>(go);
		rb->GetBody()->setIgnoreCollisionCheck(rb->GetBody(), value);
	//}
}

MonoObject* GetLinearVelocity(MonoObject* go)
{
	char* nameGo = mono_string_to_utf8(mono_object_to_string(go, 0));

	RigidBodyComponent* rb = GetComponentMono<RigidBodyComponent*>(go);
	return app->moduleMono->Float3ToCS((float3)rb->GetBody()->getLinearVelocity());
}