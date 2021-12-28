#pragma once

#include "Component.h"

class GameObject;
class TransformComponent;

class AudioReverbZoneComponent : public Component
{
public:
	AudioReverbZoneComponent(GameObject* own, TransformComponent* trans);

	~AudioReverbZoneComponent();

	bool Update(float dt) override;

	void OnEditor() override;

private:
	TransformComponent* transform;
};