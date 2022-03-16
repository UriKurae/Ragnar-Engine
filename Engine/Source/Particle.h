#pragma once
#include "Texture.h"

#include "Math/float4.h"
#include <memory>

struct Particle
{
	float3 position;
	float3 velocity, acceleration;
	float4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd;
	float rotation = 0.0f;
	float deltaRotation = rotation;

	float lifeTime = 1.0f;
	float lifeRemaining = 0.0f;

	bool active = false;
};

struct ParticleReference
{
	float3 position;
	float3 velocity, acceleration;
	float4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifeTime = 1.0f;
	float deltaRotation = 0.0f;
};