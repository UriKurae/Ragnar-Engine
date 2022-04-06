#pragma once
#include "SpawnShape.h"
#include "Particle.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneManager.h"
#include "Scene.h"
#include "Algorithm/Random/LCG.h"
#include "Globals.h"

class ParticleEffect_ConeSpawn : public SpawnShape
{
public:
	ParticleEffect_ConeSpawn();
	~ParticleEffect_ConeSpawn() override;

	void Spawn(Particle& particle, float4x4& gTrans, float* offset) override;
	void OnEditor(int emitterIndex) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array)override;

private:

	float radius, height, angle;
	bool useDirection;
};