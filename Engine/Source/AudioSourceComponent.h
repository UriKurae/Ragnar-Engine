#pragma once

#include "Component.h"


class TransformComponent;
class IndexBuffer;
class VertexBuffer;
class AudioSourceComponent : public Component
{
public:
	AudioSourceComponent(GameObject* own, TransformComponent* trans);
	~AudioSourceComponent();

	void OnEditor() override;
	bool Update(float dt) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void PlayClip();
private:

	TransformComponent* transform;

	// Audio settings
	std::string audioClip;
	bool mute;
	float volume;
	float pitch;
	float reverb;
};