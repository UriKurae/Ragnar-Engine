#pragma once
#include "Component.h"
#include "ModuleScene.h"
#include "ParticleEmitter.h"
#include "GameTimer.h"
#include "Dialogs.h"

#include "Particle.h"

class ParticleSystemComponent : public Component {

public:

	ParticleSystemComponent(GameObject* own, TransformComponent* trans, uint numParticles = 1);
	~ParticleSystemComponent();

	void SetEmitter(ParticleEmitter* emitter);
	bool Update(float dt) override;
	void Draw(CameraComponent* gameCam) override;

	void OnEditor() override;

	void Play();
	void Stop();

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

public:

	bool looping = false;
	float maxDuration = 0.0f;

protected:

	std::vector<ParticleEmitter*> emitters;
	bool isActive;
	GameTimer timer;
	TransformComponent* transform;
};

