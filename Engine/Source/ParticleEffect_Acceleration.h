#pragma once
#include "ParticleEffect.h"

class ParticleEffect_Acceleration : ParticleEffect
{
public:
	ParticleEffect_Acceleration();
	~ParticleEffect_Acceleration();

	void Update(Particle& particle, float dt) override;
	void OnEditor(int emitterIndex) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

public:

	bool hasGravity;
	float gravity;
	float3 acceleration;
};