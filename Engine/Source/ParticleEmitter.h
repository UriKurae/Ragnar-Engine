#pragma once
#include "GameObject.h"
#include "Particle.h"
#include "ParticleEffect.h"
#include "ParticleEffect_Velocity.h"
#include "ParticleEffect_Acceleration.h"

#include "Particle.h"

#define INSTANCE_DATA_LENGHT 20

class Shader;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

class ParticleEmitter {

public:

	ParticleEmitter(GameObject* owner);
	~ParticleEmitter();

	virtual void Emit(float dt, const ParticleProps& props);
	void Render(CameraComponent* gameCam);
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

	void StartBatch();
	void NextBatch();
	void DrawParticle(const float3& pos, float rotation, const float3& size);
public:

	int maxParticles;
	float particlesPerSecond;
	bool isActive, toDelete;

	std::vector<ParticleEffect*> effects;
	//std::vector<Particle> particlesBuff;
	//Particle* particleReference;
	GameObject* own;

	float minLifeTime;
	float maxLifeTime;

	float timer;
	float currTimer;

private:

	struct EmitterData
	{
		unsigned int maxQuads;
		unsigned int maxVertices;
		unsigned int maxIndices;

		VertexArray* vertexArray;
		VertexBuffer* vertexBuffer;
		IndexBuffer* indexBuffer;

		std::shared_ptr<Shader> shader;

		unsigned int indexCount = 0;
		Vertex* vertexBufferBase = nullptr;
		Vertex* vertexBufferPtr = nullptr;
		
		float4 vertexPositions[4];

	};
	EmitterData data;

	std::vector<Particle> particlePool;
	unsigned int poolIndex;


	std::string emitterName;
	char charsOfName[50];
	bool showTexMenu;
	LCG random;

	//unsigned int VAO;
	//unsigned int instanceVBO;
	//unsigned int vertexVBO;

	
};

const float particleVertices[] = {
-1, -1,
1, -1,
-1, 1,
1, -1,
1, 1,
-1, 1,
};
