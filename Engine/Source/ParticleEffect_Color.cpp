#include "ParticleEffect_Color.h"
#include "Globals.h"

#include <Imgui/imgui.h>

#include "Profiling.h"

ParticleEffect_Color::ParticleEffect_Color() : ParticleEffect(ParticleEffectType::COLOR_OVER_LIFETIME),
startColor({ 1.0f,0.0f,0.0f,1.0f }),
endColor({ 0.0f,1.0f,0.0f,1.0f })
{
}

ParticleEffect_Color::~ParticleEffect_Color()
{
}

void ParticleEffect_Color::Update(Particle& particle, float dt)
{
	RG_PROFILING_FUNCTION("Color Effect");

	float porcentage = particle.lifeRemaining / particle.lifeTime;
	float4 color = float4::Lerp(endColor, startColor, porcentage);
	particle.color = color;
}

void ParticleEffect_Color::OnEditor(int emitterIndex)
{
	std::string suffixLabel = "Color Over Lifetime Effect##";
	suffixLabel += emitterIndex;
	if (ImGui::CollapsingHeader(suffixLabel.c_str(), ImGuiTreeNodeFlags_Bullet))
	{
		suffixLabel = "Delete Color Over Lifetime Effect##";
		suffixLabel += emitterIndex;
		if (ImGui::Button(ICON_FA_TRASH" Delete Effect##"))
			this->toDelete = true;

		ImGui::Spacing();
		ImGui::Indent();

		suffixLabel = "Start Color##ColorOverLife";
		suffixLabel += emitterIndex;
		ImGui::PushItemWidth(150);
		ImGui::ColorEdit4(suffixLabel.c_str(), startColor.ptr());
		ImGui::PopItemWidth();

		suffixLabel = "End Color##ColorOverLife";
		suffixLabel += emitterIndex;
		ImGui::PushItemWidth(150);
		ImGui::ColorEdit4(suffixLabel.c_str(), endColor.ptr());
		ImGui::PopItemWidth();

		ImGui::Unindent();
	}
}

bool ParticleEffect_Color::OnLoad(JsonParsing& node)
{
	startColor = node.GetJson4Number(node, "PEC: start color");
	endColor = node.GetJson4Number(node, "PEC: start color");
	return true;
}

bool ParticleEffect_Color::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJson4Number(file, "PEC: start color", startColor);
	file.SetNewJson4Number(file, "PEC: start color", endColor);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Effect Type", (int)type);
	node.SetValueToArray(array, file.GetRootValue());
	return true;
}
