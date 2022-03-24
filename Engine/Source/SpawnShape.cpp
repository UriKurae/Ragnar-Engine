#include "SpawnShape.h"

SpawnShape::SpawnShape(SPAWN_SHAPE_TYPE type) : shapeType(type)
{
}

SPAWN_SHAPE_TYPE SpawnShape::GetType() const
{
	return shapeType;
}
