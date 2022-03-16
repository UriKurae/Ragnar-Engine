#include "ParticleEffect_Size.h"
#include "Globals.h"

#include <Imgui/imgui.h>

ParticleEffect_Size::ParticleEffect_Size() : ParticleEffect(ParticleEffectType::SIZE_OVER_LIFETIME),
	startSize(0.5f),
	endSize(1.0f)
{
}

ParticleEffect_Size::~ParticleEffect_Size()
{
}

void ParticleEffect_Size::Update(Particle& particle, float dt)
{
	if (particle.lifeTime != 0.0f)
	{
		float percentatge = particle.lifeRemaining / particle.lifeTime;
		particle.size = (1 - percentatge) * endSize + percentatge * startSize;
	}
}

void ParticleEffect_Size::OnEditor(int emitterIndex)
{
	std::string suffixLabel = "Size Over Lifetime Effect##";
	suffixLabel += emitterIndex;
	if (ImGui::CollapsingHeader(suffixLabel.c_str(), ImGuiTreeNodeFlags_Bullet))
	{
		suffixLabel = "Delete Size Over Lifetime Effect##";
		suffixLabel += emitterIndex;
		if (ImGui::Button(ICON_FA_TRASH" Delete Effect##"))
			this->toDelete = true;

		ImGui::Spacing();
		ImGui::Indent();

		suffixLabel = "Start Size##SizeOverLife";
		suffixLabel += emitterIndex;
		ImGui::DragFloat(suffixLabel.c_str(), &startSize, 0.1f, 0.0f, FLT_MAX);

		suffixLabel = "End Size##SizeOverLife";
		suffixLabel += emitterIndex;
		ImGui::DragFloat(suffixLabel.c_str(), &endSize, 0.1f, 0.0f, FLT_MAX);

		ImGui::Unindent();
	}
}

bool ParticleEffect_Size::OnLoad(JsonParsing& node)
{
	startSize = node.GetJsonNumber("PES: Start size");
	endSize = node.GetJsonNumber ("PES: End size");
	return true;
}

bool ParticleEffect_Size::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PES: Start size", startSize);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PES: End size", endSize);
	return true;
}
