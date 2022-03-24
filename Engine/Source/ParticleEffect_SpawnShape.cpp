#include "ParticleEffect_SpawnShape.h"

ParticleEffect_SpawnShape::ParticleEffect_SpawnShape(SPAWN_SHAPE_TYPE type) : shapeType(type)
{
}

SPAWN_SHAPE_TYPE ParticleEffect_SpawnShape::GetType() const
{
	return shapeType;
}
