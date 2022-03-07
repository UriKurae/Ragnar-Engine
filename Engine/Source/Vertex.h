#pragma once

#include "Math/float3.h"
#include "Math/float2.h"

struct Vertex
{
	float3 position;
	float3 normal;
	float2 texCoords;

	int boneIDs[4] = { -1, -1, -1, -1};
	float weights[4];
};