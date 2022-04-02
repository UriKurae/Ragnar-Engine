#pragma once
#include "ScriptComponent.h"
#include "CameraComponent.h"
#include "ModuleCamera3D.h"

#include "Math/float3x3.h"
#include "Geometry/LineSegment.h"

#include <metadata\object-forward.h>
#include <metadata\object.h>
#include <metadata/class.h>

void LookAt(MonoObject* go, MonoObject* transformToLook)
{
	float3 target = app->moduleMono->UnboxVector(transformToLook);

	Frustum* cameraFrustum = GetComponentMono<CameraComponent*>(go)->GetFrustum();
	float3 directionFrustum = target - cameraFrustum->Pos();
	directionFrustum.Normalize();

	float3x3 lookAt = float3x3::LookAt(cameraFrustum->Front(), directionFrustum, cameraFrustum->Up(), float3(0.0f, 1.0f, 0.0f));
	cameraFrustum->SetFront(lookAt.MulDir(cameraFrustum->Front()).Normalized());
	cameraFrustum->SetUp(lookAt.MulDir(cameraFrustum->Up()).Normalized());
}

void ChangeFov(MonoObject* go, float newFov)
{
	CameraComponent* camComp = GetComponentMono<CameraComponent*>(go);
	camComp->SetNewFov(DegToRad(newFov));
	camComp->UpdateFov();
	camComp->CompileBuffers();
}

bool HitToTag(MonoObject* initPos, MonoObject* endPos, MonoObject* tag)
{
	float3 pointA = app->moduleMono->UnboxVector(initPos);
	float3 pointB = app->moduleMono->UnboxVector(endPos);
	char* tagName = mono_string_to_utf8(mono_object_to_string(tag, 0));
	LineSegment picking(pointA, pointB);

	std::stack<QuadtreeNode*> nodes;
	app->sceneManager->GetCurrentScene()->GetQuadtree().CollectNodes(nodes, picking);
	std::vector<GameObject*> gameObjects;
	app->sceneManager->GetCurrentScene()->GetQuadtree().CollectGo(gameObjects, nodes);

	std::map<float, GameObject*> triangleMap;
	float3 hit;
	app->camera->ThrowRayCastOnlyOBB(gameObjects, picking, triangleMap, hit);

	// Throw Ray from enemy head to player head
	if (!triangleMap.empty() && (*triangleMap.begin()).second->tag == tagName)
		return true;
	// If don't match, Throw Ray from enemy head to player feets
	else
	{
		triangleMap.clear();
		picking.b.y = 0.2f;
		app->camera->ThrowRayCastOnlyOBB(gameObjects, picking, triangleMap, hit);
		if (!triangleMap.empty() && (*triangleMap.begin()).second->tag == tagName)
			return true;
	}

	return false;
}