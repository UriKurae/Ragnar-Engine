#include "ParticleEffect_Rotation.h"

ParticleEffect_Rotation::ParticleEffect_Rotation() : ParticleEffect(ParticleEffectType::ROTATION_OVER_LIFETIME)
{
}

ParticleEffect_Rotation::~ParticleEffect_Rotation()
{
}

void ParticleEffect_Rotation::Init(Particle& particle)
{
}

void ParticleEffect_Rotation::Update(Particle& particle, float dt)
{
}

void ParticleEffect_Rotation::OnEditor(int emitterIndex)
{
}

bool ParticleEffect_Rotation::OnLoad(JsonParsing& node)
{
	return false;
}

bool ParticleEffect_Rotation::OnSave(JsonParsing& node, JSON_Array* array)
{
	return false;
}
