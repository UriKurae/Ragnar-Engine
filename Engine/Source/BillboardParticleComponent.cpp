#include "BillboardParticleComponent.h"

BillboardParticleComponent::BillboardParticleComponent(GameObject* own, TransformComponent* trans)
{
	type = ComponentType::BILLBOARD;
	owner = own;
	transform = trans;
}

BillboardParticleComponent::~BillboardParticleComponent()
{
}

bool BillboardParticleComponent::Update(float dt)
{
	return true;
}

Quat BillboardParticleComponent::RotateToFaceCamera()
{
	// TODO
	return Quat();
}

void BillboardParticleComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Billboard"))
	{
		ImGui::PushItemWidth(90);
		ImGui::Separator();
	}
}
