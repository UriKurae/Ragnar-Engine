#pragma once
#include "Component.h"
#include "TransformComponent.h"

class BillboardParticleComponent : public Component {

public:

	BillboardParticleComponent(GameObject* own, TransformComponent* trans);
	~BillboardParticleComponent();

	bool Update(float dt) override;
	Quat RotateToFaceCamera(); //Billboard particles are flat textured quads that are rotated to always face the camera

	void OnEditor() override;

public:

	TransformComponent* transform;
};
