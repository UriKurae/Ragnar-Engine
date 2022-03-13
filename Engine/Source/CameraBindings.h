#pragma once

#include "ScriptComponent.h"

#include "CameraComponent.h"

#include "Math/float3x3.h"

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