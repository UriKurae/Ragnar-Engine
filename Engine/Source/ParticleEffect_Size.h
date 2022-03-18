#pragma once
#include "ParticleEffect.h"\
#include <Imgui/imgui.h>

class ParticleEffect_Size : public ParticleEffect
{
public:
	ParticleEffect_Size();
	~ParticleEffect_Size();

	void Update(Particle& particle, float dt) override;
	void OnEditor(int emitterIndex) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

private:

	float startSize;
	float endSize;
};