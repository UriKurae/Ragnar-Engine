#include "ParticleEffect_SphericalSpawn.h"
#include <Globals.h>

ParticleEffect_SphericalSpawn::ParticleEffect_SphericalSpawn() : SpawnShape(SPAWN_SHAPE_TYPE::SPHERE),
	radius(1.0f)
{
}

ParticleEffect_SphericalSpawn::~ParticleEffect_SphericalSpawn()
{
}

void ParticleEffect_SphericalSpawn::Spawn(Particle& particle, bool hasInitialSpeed, float speed, float4x4& gTrans, float* offset)
{
	LCG random;

	float u = random.Float(-radius, radius);
	float x1 = random.Float(-radius, radius);
	float x2 = random.Float(-radius, radius);
	float x3 = random.Float(-radius, radius);

	float mag = sqrt(x1 * x1 + x2 * x2 + x3 * x3);
	x1 /= mag; 
	x2 /= mag; 
	x3 /= mag;

	float c = cbrt(u);
	float3 localPos;

	localPos.x = offset[0] + x1 * c;
	localPos.y = offset[1] + x2 * c;
	localPos.z = offset[2] + x3 * c;
	particle.position = gTrans.TransformPos(localPos);
}

void ParticleEffect_SphericalSpawn::OnEditor(int emitterIndex)
{
	std::string suffixLabel = "Radius##SphereSpawn";
	suffixLabel += emitterIndex;
	ImGui::DragFloat(suffixLabel.c_str(), &radius);

	suffixLabel = "Face Direction##ConeShape";
	suffixLabel += emitterIndex;
}

bool ParticleEffect_SphericalSpawn::OnLoad(JsonParsing& node)
{
	radius = node.GetJsonNumber("PESpawn: Radius");
	//angle = node.GetJsonNumber("PESpawn: Angle");
	//useDirection = node.GetJsonBool("PESpawn: Use Direction");

	return true;
}

bool ParticleEffect_SphericalSpawn::OnSave(JsonParsing& node, JSON_Array* array)
{
	//JsonParsing file = JsonParsing();

	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "PESpawn: Radius", radius);
	//file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PESpawn: Angle", angle);
	//file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "PESpawn: Use Direction", useDirection);

	//node.SetValueToArray(array, node.GetRootValue());
	return true;
}