#include "ParticleEffect_Acceleration.h"
#include "Globals.h"

#include "imgui/imgui.h"

#include "Profiling.h"

ParticleEffect_Acceleration::ParticleEffect_Acceleration() : ParticleEffect(ParticleEffectType::ACCELERATION_OVER_LIFETIME)
{
	type = ParticleEffectType::ACCELERATION_OVER_LIFETIME;
	gravity = -5.0f;
	acceleration = float3(0.0f, 0.0f, 0.0f);
}

ParticleEffect_Acceleration::~ParticleEffect_Acceleration()
{
}

void ParticleEffect_Acceleration::Update(Particle& particle, float dt)
{
	RG_PROFILING_FUNCTION("Acceleration Effect");

	particle.acceleration = acceleration;
	if (hasGravity)
	{
		particle.acceleration.y += gravity;
	}

		//Add acceleration to velocity
		particle.velocity.x += particle.acceleration[0] * dt;
		particle.velocity.y += particle.acceleration[1] * dt;
		particle.velocity.z += particle.acceleration[2] * dt;
	
		//Update position based on particle's velocity
		particle.position.x += particle.velocity[0] * dt;
		particle.position.y += particle.velocity[1] * dt;
		particle.position.z += particle.velocity[2] * dt;
}

void ParticleEffect_Acceleration::OnEditor(int emitterIndex)
{
	float acc[3] = { acceleration[0], acceleration[1], acceleration[2] };
	float minAcc = -0.1f;
	float maxAcc = 0.1f;

	std::string suffixLabel = "Acceleration Over Lifetime Effect##";
	suffixLabel += emitterIndex;
	if (ImGui::CollapsingHeader(suffixLabel.c_str(), ImGuiTreeNodeFlags_Bullet))
	{
		suffixLabel = "Delete Effect##";
		suffixLabel += emitterIndex;

		if (ImGui::Button(ICON_FA_TRASH" Delete Effect"))
			this->toDelete = true;

		ImGui::Spacing();
		ImGui::Indent();

		ImGui::Checkbox("Gravity", &hasGravity);

		suffixLabel = "Acceleration";
		suffixLabel += emitterIndex;
		ImGui::PushItemWidth(200);
		ImGui::DragFloat3(suffixLabel.c_str(), acc, 0.01f, minAcc, maxAcc);
		ImGui::PopItemWidth();

		acceleration[0] = acc[0];
		acceleration[1] = acc[1];
		acceleration[2] = acc[2];

		ImGui::Unindent();
	}
}

bool ParticleEffect_Acceleration::OnLoad(JsonParsing& node)
{
	hasGravity = node.GetJsonBool("PEA: Has Gravity");
	gravity = node.GetJsonNumber("PEA: Gravity");
	acceleration = node.GetJson3Number(node, "PEA: Acceleration");
	return true;
}

bool ParticleEffect_Acceleration::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "PEA: Has Gravity", hasGravity);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PEA: Gravity", gravity);
	file.SetNewJson3Number(file, "PEA: Acceleration", acceleration);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Effect Type", (int)type);
	node.SetValueToArray(array, file.GetRootValue());
	return true;
}