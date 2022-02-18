#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(GameObject* owner)
{
	particlesBuff.resize(0);
	maxParticles = 200;
	particlesPerSecond = 50;
	isActive = true;

	own = owner;
	particleReference = new Particle(own);
	//effects.resize(5);

	timer = 1.0f / particlesPerSecond;
	currTimer = timer;

	showTexMenu = false;

	minLifeTime = 0.7f;
	maxLifeTime = 1.7f;
}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::Emit(float dt)
{
	currTimer -= dt;
	if (currTimer <= 0.0f) {

		for (int i = 0; i < particlesBuff.size(); i++) {
			// When the particle is allocated in memory, but it's not being used at the moment
			// Reuse an exisiting particle to make the smaller complexity, which results in more optimized code 

			TransformComponent* transform = (TransformComponent*)own->GetComponent(ComponentType::TRANSFORM);
			if (transform != nullptr)
			{
				if (particlesBuff[i]->isActive == false)
				{
					particlesBuff[i]->isActive = true;
					particlesBuff[i]->position = transform->position;
					particlesBuff[i]->rotation = particleReference->rotation;
					particlesBuff[i]->size = particleReference->size;
					particlesBuff[i]->color = particleReference->color;
					particlesBuff[i]->lifeTime = particleReference->lifeTime;

					SetParticleTexture(*particlesBuff[i]);

					/* TODO: Particle Effects
					* 
					for (int j = 0; j < effects.size(); j++)
					{
						if (effects[j] != nullptr)
						{
							effects[j]->Init(*particlesBuff[i]);
						}
					}*/
					return;
				}
			}
		}

		if (particlesBuff.size() < maxParticles) {
			// Create new particle
			Particle* particle = new Particle(particleReference, own);
			particlesBuff.push_back(particle);
			SetParticleTexture(*particle);
		}
		currTimer = timer;
	}
}

void ParticleEmitter::Render()
{

}

void ParticleEmitter::UpdateParticle(float dt)
{
}

void ParticleEmitter::Update(float dt)
{
}

void ParticleEmitter::OnEditor(int emitterIndex)
{
}

void ParticleEmitter::SetParticlesPerSecond(float particlesPerSec)
{
}

bool ParticleEmitter::OnLoad(JsonParsing& node)
{
	return false;
}

bool ParticleEmitter::OnSave(JsonParsing& node, JSON_Array* array)
{
	return false;
}

void ParticleEmitter::SetParticleTexture(Particle& particle)
{
}
