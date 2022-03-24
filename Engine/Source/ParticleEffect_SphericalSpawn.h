#pragma once
#include "SpawnShape.h"
#include "Particle.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneManager.h"
#include "Scene.h"
#include "Algorithm/Random/LCG.h"
#include "Math/float4x4.h"

class ParticleEffect_SphericalSpawn : public SpawnShape
{
public:

	ParticleEffect_SphericalSpawn();
	~ParticleEffect_SphericalSpawn() override;

	void Spawn(Particle& particle, bool hasInitialSpeed, float speed, float4x4& gTrans, float* offset) override;
	void OnEditor(int emitterIndex) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array)override;

private:

	float radius, angle;
	bool useDirection;
};