#pragma once
#include "ParticleEffect.h";
#include "TransformComponent.h"
#include "SpawnShape.h"
#include "ParticleEffect_SphericalSpawn.h"
#include "ParticleEffect_ConeSpawn.h"
#include "Algorithm/Random/LCG.h"

class ParticleEffect_SpawningShape : public ParticleEffect
{
public:
	ParticleEffect_SpawningShape(TransformComponent* transform);
	~ParticleEffect_SpawningShape();

	void Init(Particle& particle) override;
	void OnEditor(int emitterIndex) override;

	SpawnShape* ChangeSpawnShape(SPAWN_SHAPE_TYPE newType);

	SpawnShape* CreateSpawnShape(SPAWN_SHAPE_TYPE newType);
	void GetShapeTypeString(SPAWN_SHAPE_TYPE newType, std::string& out);

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array)override;

private:

	float shapeOffset[3];// (x, y, z)

	TransformComponent* transformComponent;
	SpawnShape* spawnShape;
};