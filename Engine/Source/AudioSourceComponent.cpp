#include "Globals.h"

#include "Application.h"
#include "ModuleScene.h"
#include "AudioSourceComponent.h"
#include "AudioManager.h"
#include "TransformComponent.h"
#include "GameObject.h"

#include "IconsFontAwesome5.h"

#include "Imgui/imgui.h"

AudioSourceComponent::AudioSourceComponent(GameObject* own, TransformComponent* trans) : volume(50.0f), mute(false), transform(trans), pitch(0.0f), playingID(-1), playOnAwake(false)
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

	audioClip.push_back("None");
}

AudioSourceComponent::~AudioSourceComponent()
{
	AudioManager::Get()->DeleteAudioSource(this);
	AudioManager::Get()->UnregisterGameObject(owner->GetUUID());
}

void AudioSourceComponent::OnEditor()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader(ICON_FA_VOLUME_UP" AudioSource"))
	{
		if (ImGui::TreeNodeEx("Clips"))
		{
			for (int i = 0; i < audioClip.size(); ++i)
			{
				ImGui::PushID((void*)audioClip[i].c_str());
				if (ImGui::BeginCombo("##AudioClip", audioClip[i].c_str()))
				{
					if (ImGui::Selectable("None"))
					{
						audioClip[i] = "None";
					}
					std::vector<std::string> events = AudioManager::Get()->GetEventsList();
					for (int j = 0; j < events.size(); j++)
					{
						if (ImGui::Selectable(events[j].c_str()))
						{
							audioClip[i] = events[j];
						}
					}
					ImGui::EndCombo();
				}
				ImGui::PopID();
			}

			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_PLUS))
			{
				audioClip.push_back("None");
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_MINUS))
			{
				audioClip.erase(audioClip.end()-1);
			}

			ImGui::TreePop();
		}

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
			PlayClip(audioClip[0].c_str());
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

		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Delete").x - 25);
		if (ImGui::Button(ICON_FA_TRASH" Delete"))
			owner->RemoveComponent(this);

		ImGui::Separator();
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
	// Register this audio source
	if (!owner->CheckAudioRegister())
	{
		AkGameObjectID cameraID = owner->GetUUID();
		AudioManager::Get()->RegisterGameObject(cameraID);
		owner->SetAudioRegister(true);
	}

	JSON_Array* clips = node.GetJsonArray(node.ValueToObject(node.GetRootValue()), "AudioClips");
	int size = node.GetJsonArrayCount(clips);
	if (size > 0)
	{
		audioClip.clear();
		for (int i = 0; i < size; ++i)
		{
			JsonParsing clip = node.GetJsonArrayValue(clips, i);
			audioClip.push_back(clip.GetJsonString(std::to_string(i).c_str()));
		}
	}
	volume = node.GetJsonNumber("Volume");
	pitch = node.GetJsonNumber("Pitch");
	mute = node.GetJsonBool("Mute");

	return true;
}

bool AudioSourceComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	//file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Audio Clip", audioClip.c_str());
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Volume", volume);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Pitch", pitch);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Mute", mute);

	JsonParsing clips = JsonParsing();
	std::string label = "0";
	JSON_Array* newArray = file.SetNewJsonArray(file.GetRootValue(), "AudioClips");
	for (int i = 0; i < audioClip.size(); ++i)
	{
		clips = JsonParsing();
		clips.SetNewJsonString(clips.ValueToObject(clips.GetRootValue()), label.c_str(), audioClip.at(i).c_str());
		label = std::to_string(i + 1);
		file.SetValueToArray(newArray, clips.GetRootValue());
	}

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void AudioSourceComponent::SwapAudioClip(const char* clipName)
{
//	audioClip = clipName;
}

void AudioSourceComponent::PlayClip(const char* clipName)
{
 	playingID = AudioManager::Get()->PostEvent(clipName, owner->GetUUID());
}

void AudioSourceComponent::PlayClipOnAwake(const char* clipName)
{
	if (playOnAwake) playingID = AudioManager::Get()->PostEvent(clipName, owner->GetUUID());
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