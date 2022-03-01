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

void BillboardParticleComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Billboard"))
	{
		ImGui::PushItemWidth(90);
		ImGui::Separator();

		std::string tempAlignment = " ";

		switch (particleAlignment) {
		case Particle_Alignment::CAMERA_ALIGNED: 
			tempAlignment = "Camera Aligned"; 
			break;
		case Particle_Alignment::WORLD_ALIGNED: 
			tempAlignment = "World Aligned"; 
			break;
		case Particle_Alignment::AXIS_ALIGNED: 
			tempAlignment = "Axis Aligned"; 
			break;
		}

		ImGui::Text("Current Alignment: %s", tempAlignment.c_str());

		if (ImGui::BeginMenu("Change particle's alignment")) {

			if (ImGui::MenuItem("Camera Aligned"))
				SetAlignment(Particle_Alignment::CAMERA_ALIGNED);
			if (ImGui::MenuItem("World Aligned"))
				SetAlignment(Particle_Alignment::WORLD_ALIGNED);
			if (ImGui::MenuItem("Axis Aligned"))
				SetAlignment(Particle_Alignment::AXIS_ALIGNED);

			ImGui::EndMenu();
		}
	}
}

void BillboardParticleComponent::Align()
{
	switch (particleAlignment)
	{
	case Particle_Alignment::CAMERA_ALIGNED:
		CameraAlign();
		break;
	case Particle_Alignment::WORLD_ALIGNED:
		WorldAlign();
		break;
	case Particle_Alignment::AXIS_ALIGNED:
		AxisAlign();
		break;
	default:
		break;
	}
}

void BillboardParticleComponent::SetAlignment(Particle_Alignment alignment)
{
	particleAlignment = alignment;
}

Quat BillboardParticleComponent::GetAlignment()
{
	switch (particleAlignment)
	{
	case Particle_Alignment::CAMERA_ALIGNED:
		return CameraAlign();
		break;
	case Particle_Alignment::WORLD_ALIGNED:
		return WorldAlign();
		break;
	case Particle_Alignment::AXIS_ALIGNED:
		return AxisAlign();
		break;
	default:
		break;
	}

	return Quat::identity;
}

Quat BillboardParticleComponent::CameraAlign()
{
	// TODO: particles always face camera
	return Quat::identity;
}

Quat BillboardParticleComponent::WorldAlign()
{
	// TODO: particles always face camera, but they can be softly rotated
	return Quat::identity;
}

Quat BillboardParticleComponent::AxisAlign()
{
	return Quat::identity;
}