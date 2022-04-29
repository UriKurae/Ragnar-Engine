#pragma once
#include "ParticleEffect.h"
#include "Algorithm/Random/LCG.h"

class ParticleEffect_Rotation : public ParticleEffect
{
public:
	ParticleEffect_Rotation();
	~ParticleEffect_Rotation();

	void Init(Particle& particle) override;
	void Update(Particle& particle, float dt) override;
	void OnEditor(int emitterIndex) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;
};
