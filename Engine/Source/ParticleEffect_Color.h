#pragma once
#include "ParticleEffect.h"
#include "JsonParsing.h"

class ParticleEffect_Color : public ParticleEffect
{
public:
	ParticleEffect_Color();
	~ParticleEffect_Color();

	void Update(Particle& particle, float dt) override;
	void OnEditor(int emitterIndex) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

public:

	float4 startColor, endColor;
};