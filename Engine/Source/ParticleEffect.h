#pragma once
#include "Particle.h"
#include "JsonParsing.h"

enum class ParticleEffectType : int
{
	NO_TYPE = -1,
	VELOCITY_OVER_LIFETIME,
	ACCELERATION_OVER_LIFETIME,
	SIZE_OVER_LIFETIME,
	COLOR_OVER_LIFETIME,
};

class ParticleEffect
{
public:
	ParticleEffect(ParticleEffectType type);
	~ParticleEffect();

	virtual void Init(Particle& particle) {}
	virtual void Update(Particle& particle, float dt) {}
	virtual void OnEditor(int emitterIndex) {}

	virtual bool OnLoad(JsonParsing& node);
	virtual bool OnSave(JsonParsing& node, JSON_Array* array);

public:

	ParticleEffectType type;
	bool toDelete;
};