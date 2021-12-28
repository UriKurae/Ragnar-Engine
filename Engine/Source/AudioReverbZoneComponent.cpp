#include "AudioReverbZoneComponent.h"

#include "GameObject.h"
#include "AudioManager.h"

AudioReverbZoneComponent::AudioReverbZoneComponent(GameObject* own, TransformComponent* trans) : transform(trans), Component()
{
	owner = own;
	type = ComponentType::AUDIO_REVERB_ZONE;

	// Register this audio source
	if (!owner->CheckAudioRegister())
	{
		AkGameObjectID cameraID = owner->GetUUID();
		AudioManager::Get()->RegisterGameObject(cameraID);
		owner->SetAudioRegister(true);
	}
}

AudioReverbZoneComponent::~AudioReverbZoneComponent()
{
}

bool AudioReverbZoneComponent::Update(float dt)
{
	float3 position = transform->GetPosition();
	AkSoundPosition audioSourcePos;
	audioSourcePos.SetPosition(position.x, position.y, position.z);
	float3 orientation = transform->GetRotation().ToEulerXYZ().Normalized();
	//audioSourcePos.SetOrientation({orientation.x, orientation.y, orientation.z}, { orientation.x, orientation.y, orientation.z });
	audioSourcePos.SetOrientation({ 0, 0, -1 }, { 0,1,0 });
	AudioManager::Get()->SetPosition(owner->GetUUID(), audioSourcePos);
	//DEBUG_LOG("Source: x %f, y %f, z %f", position.x, position.y, position.z);

	return true;
}

void AudioReverbZoneComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Reverb Zone"))
	{

	}
}