#include "Globals.h"

#include "Application.h"
#include "ModuleScene.h"
#include "AudioSourceComponent.h"
#include "AudioManager.h"
#include "TransformComponent.h"
#include "GameObject.h"

#include "Imgui/imgui.h"

AudioSourceComponent::AudioSourceComponent(GameObject* own, TransformComponent* trans) : audioClip("None"), volume(50.0f), mute(false), transform(trans), pitch(0.0f), playingID(-1), playOnAwake(false)
{
	owner = own;
	type = ComponentType::AUDIO_SOURCE;
	
	AudioManager::Get()->AddAudioSource(this);
	// Register this audio source
	if (!owner->CheckAudioRegister())
	{
		AkGameObjectID cameraID = owner->GetUUID();
		AudioManager::Get()->RegisterGameObject(cameraID);
		owner->SetAudioRegister(true);
	}
}

AudioSourceComponent::~AudioSourceComponent()
{
	AudioManager::Get()->DeleteAudioSource(this);
	AudioManager::Get()->UnregisterGameObject(owner->GetUUID());
}

void AudioSourceComponent::OnEditor()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("AudioSource"))
	{
		ImGui::Text("AudioClip");
		ImGui::SameLine();
		if (ImGui::BeginCombo("##AudioClip", audioClip.c_str()))
		{
			if (ImGui::Selectable("None"))
			{
				audioClip = "None";
			}
			std::vector<std::string> events = AudioManager::Get()->GetEventsList();
			for (int i = 0; i < events.size(); i++)
			{
				if (ImGui::Selectable(events[i].c_str()))
				{
					audioClip = events[i];
				}
			}

			ImGui::EndCombo();
		}
		//ImGui::InputText("##Audio", &audioClip[0], audioClip.size());

		ImGui::Text("Mute");
		ImGui::SameLine();
		if (ImGui::Checkbox("##Mute", &mute))
		{
			if (mute) AK::SoundEngine::SetRTPCValue("Volume", 0, owner->GetUUID());
			else AK::SoundEngine::SetRTPCValue("Volume", volume, owner->GetUUID());
			DEBUG_LOG("THE ID IS %d", owner->GetUUID());
		}

		ImGui::Text("Volume");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##Volume", &volume, 0.0f, 100.0f))
		{
			AK::SoundEngine::SetRTPCValue("Volume", volume, owner->GetUUID());
			mute = false;
		}

		ImGui::Text("Pitch");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##Pitch", &pitch, -2400.0f, 2400.0f))
		{
			AK::SoundEngine::SetRTPCValue("Pitch", pitch, owner->GetUUID());
		}

		ImGui::Text("Play On Awake");
		ImGui::SameLine();
		ImGui::Checkbox("##Play On Awake", &playOnAwake);

		if (ImGui::Button("Play"))
		{
			PlayClip();
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			PauseClip();
		}
		ImGui::SameLine();
		if (ImGui::Button("Resume"))
		{
			ResumeClip();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			StopClip();
		}
		
	}
	ImGui::PopID();
}

bool AudioSourceComponent::Update(float dt)
{
	float3 position = transform->GetPosition();
	AkSoundPosition audioSourcePos;
	audioSourcePos.SetPosition(position.x, position.y, position.z);
	float3 orientation = transform->GetRotation().ToEulerXYZ().Normalized();
	//audioSourcePos.SetOrientation({orientation.x, orientation.y, orientation.z}, { orientation.x, orientation.y, orientation.z });
	audioSourcePos.SetOrientation({0, 0, -1}, { 0,1,0 });
	AudioManager::Get()->SetPosition(owner->GetUUID(), audioSourcePos);
	
	//DEBUG_LOG("Source: x %f, y %f, z %f", position.x, position.y, position.z);
	AudioManager::Get()->CheckReverbGameObject(owner->GetUUID());

	return true;
}

bool AudioSourceComponent::OnLoad(JsonParsing& node)
{
	audioClip = node.GetJsonString("Audio Clip");
	volume = node.GetJsonNumber("Volume");
	pitch = node.GetJsonNumber("Pitch");
	mute = node.GetJsonBool("Mute");

	return true;
}

bool AudioSourceComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Audio Clip", audioClip.c_str());
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Volume", volume);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Pitch", pitch);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Mute", mute);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void AudioSourceComponent::PlayClip()
{
 	playingID = AudioManager::Get()->PostEvent(audioClip.c_str(), owner->GetUUID());
}

void AudioSourceComponent::PlayClipOnAwake()
{
	if (playOnAwake) playingID = AudioManager::Get()->PostEvent(audioClip.c_str(), owner->GetUUID());
}

void AudioSourceComponent::StopClip()
{
	AK::SoundEngine::StopPlayingID(playingID);
}

void AudioSourceComponent::PauseClip()
{
	AK::SoundEngine::ExecuteActionOnPlayingID(AK::SoundEngine::AkActionOnEventType_Pause, playingID);
}

void AudioSourceComponent::ResumeClip()
{
	AK::SoundEngine::ExecuteActionOnPlayingID(AK::SoundEngine::AkActionOnEventType_Resume, playingID);
}
