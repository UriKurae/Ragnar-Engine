#include "ParticleEffect_ConeSpawn.h"

ParticleEffect_ConeSpawn::ParticleEffect_ConeSpawn() : SpawnShape(SPAWN_SHAPE_TYPE::CONE),
height(1.0f),
radius(1.0f)
{}

ParticleEffect_ConeSpawn::~ParticleEffect_ConeSpawn()
{}

void ParticleEffect_ConeSpawn::Spawn(Particle& particle, float3& gTrans, float* offset)
{
	LCG random;

	float h = height * random.Float(0.0f, 1.0f);
	float r = (radius / height) * h * sqrt(random.Float(0.0f, 1.0f));
	float t = 2 * M_PI * random.Float(0.0f, 1.0f);

	float3 localPos;
	localPos.x = (r * cos(t)) + offset[0];
	localPos.y = h + offset[1];
	localPos.z = (r * sin(t)) + offset[2];
	particle.position = localPos;
}

void ParticleEffect_ConeSpawn::OnEditor(int emitterIndex)
{
	std::string suffixLabel = "Cone Height##PaShapeCone";
	suffixLabel += emitterIndex;
	ImGui::DragFloat(suffixLabel.c_str(), &height);

	suffixLabel = "Cone Radius##PaShapeCone";
	suffixLabel += emitterIndex;
	ImGui::DragFloat(suffixLabel.c_str(), &radius);

	suffixLabel = "Face Direction##PaShapeCone";
	suffixLabel += emitterIndex;
}

bool ParticleEffect_ConeSpawn::OnLoad(JsonParsing& node)
{
	radius = node.GetJsonNumber("PESpawnCone: Radius");
	height = node.GetJsonNumber("PESpawnCone: Height");
	return true;
}

bool ParticleEffect_ConeSpawn::OnSave(JsonParsing& node, JSON_Array* array)
{
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "PESpawnCone: Radius", radius);
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "PESpawnCone: Height", height);
	return true;
}
