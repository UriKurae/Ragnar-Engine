#pragma once
#include "GameObject.h"
#include "Particle.h"
#include "ParticleEffect.h"
#include "ParticleEffect_Velocity.h"
#include "ParticleEffect_Acceleration.h"

#define INSTANCE_DATA_LENGHT 20

class ParticleEmitter {

public:

	ParticleEmitter(GameObject* owner);
	~ParticleEmitter();

	virtual void Emit(float dt);
	void Render();
	virtual void UpdateParticle(float dt);
	virtual void Update(float dt);

	void OnEditor(int emitterIndex);
	void SetParticlesPerSecond(float particlesPerSec);

	void CreateParticleEffect(ParticleEffectType type);
	bool isEffectActive(ParticleEffectType type);
	std::string GetNameFromEffect(ParticleEffectType type);
	ParticleEffect* GetParticleEffect(ParticleEffectType type);

	virtual bool OnLoad(JsonParsing& node);
	virtual bool OnSave(JsonParsing& node, JSON_Array* array);

protected:

	void AddInstancedAttribute(unsigned int vao, unsigned int vbo, int attributeIndex, int dataSize, int instancedDataLength, int offset);

public:

	int maxParticles;
	float particlesPerSecond;
	bool isActive, toDelete;

	std::vector<ParticleEffect*> effects;
	std::vector<Particle> particlesBuff;
	Particle* particleReference;
	GameObject* own;

	float minLifeTime;
	float maxLifeTime;

	float timer;
	float currTimer;

private:

	std::string emitterName;
	char charsOfName[50];
	bool showTexMenu;
	LCG random;

	unsigned int VAO;
	unsigned int instanceVBO;
	unsigned int vertexVBO;
};

const float particleVertices[] = {
-1, -1,
1, -1,
-1, 1,
1, -1,
1, 1,
-1, 1,
};
