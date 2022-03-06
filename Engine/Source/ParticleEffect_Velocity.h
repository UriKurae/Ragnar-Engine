#pragma once
#include "ParticleEffect.h"
#include "Algorithm/Random/LCG.h"

class ParticleEffect_Velocity : ParticleEffect
{
public:
	ParticleEffect_Velocity();
	~ParticleEffect_Velocity();

	void Init(Particle& particle) override;
	void Update(Particle& particle, float dt) override;
	void OnEditor(int emitterIndex) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

public:

	float3 minVelocity, maxVelocity;
	LCG random;
};
