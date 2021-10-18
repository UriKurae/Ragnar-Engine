#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	position = { 0.0f, 0.0f, 0.0f }; 
	rotation = { 0.0f, 1.0f, 1.0f, 1.0f };
	scale = { 1.0f, 1.0f, 1.0f };
}

TransformComponent::~TransformComponent()
{
}

bool TransformComponent::Update(float dt)
{
	return true;
}

void TransformComponent::SetTransform(float3 pos, Quat rot, float3 sca)
{
	position = pos;
	rotation = rot;
	scale = sca;
}