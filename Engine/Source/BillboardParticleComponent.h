#pragma once
#include "Component.h"
#include "TransformComponent.h"
#include "Application.h" 
#include "ModuleCamera3D.h"

enum class Particle_Alignment{
	CAMERA_ALIGNED,
	WORLD_ALIGNED,
	AXIS_ALIGNED
};

class BillboardParticleComponent : public Component {

public:

	BillboardParticleComponent(GameObject* own, TransformComponent* trans);
	~BillboardParticleComponent();

	bool Update(float dt) override;
	void OnEditor() override;

	void Align();
	void SetAlignment(Particle_Alignment alignment);
	Quat GetAlignment();

private:

	Quat CameraAlign();
	Quat WorldAlign();
	Quat AxisAlign();

public:

	TransformComponent* transform;

private:

	Particle_Alignment particleAlignment;
};
