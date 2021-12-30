#include "AudioManager.h"
#include "TransformComponent.h"
#include "ListenerComponent.h"

#include "Globals.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>

#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>  

#include <AK/SoundEngine/Common/AkSoundEngine.h>

#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>

#include <AK/SoundEngine/Common/IAkPlugin.h>
#include <AK/Plugin/AkRoomVerbFXFactory.h>

#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif

#include <fstream>

#include "Profiling.h"

AudioManager* AudioManager::instance = nullptr;

void AudioManager::Create()
{
	if (instance == nullptr) instance = new AudioManager();
}

void AudioManager::Release()
{
	// Cleaning all the systems
#ifndef AK_OPTIMIZED
	// Finishing the communication system
	AK::Comm::Term();
#endif

	// Finishing the music engine
	AK::MusicEngine::Term();

	// Finishing the sound engine
	AK::SoundEngine::Term();

	// Finishing the low level I/O system
	instance->lowLevelIO.Term();

	// Finishing the stream manager
	if (AK::IAkStreamMgr::Get()) AK::IAkStreamMgr::Get()->Destroy();

	// Finishing the memory manager
	AK::MemoryMgr::Term();

	RELEASE(instance);
}

bool AudioManager::Init()
{
	// Initializing the memory manager
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		DEBUG_LOG("Couldn't create the Memory Manager");
		return false;
	}

	// Initializing the stream manager
	AkStreamMgrSettings streamSettings;
	AK::StreamMgr::GetDefaultSettings(streamSettings);

	if (!AK::StreamMgr::Create(streamSettings))
	{
		DEBUG_LOG("Couldn't create the Stream Manager");
		return false;
	}

	// Initializing the streaming device and the Low Level I/O system
	AkDeviceSettings devSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(devSettings);

	if (lowLevelIO.Init(devSettings) != AK_Success)
	{
		DEBUG_LOG("Couldn't create the streaming device and Low-Level I/O system");
		return false;
	}

	// Initializing
	AkInitSettings initSettings;
	AkPlatformInitSettings platfInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platfInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platfInitSettings) != AK_Success)
	{
		DEBUG_LOG("Couldn't initialize the sound engine");
		return false;
	}

	// Initializing the music engine
	AkMusicSettings musicSettings;
	AK::MusicEngine::GetDefaultInitSettings(musicSettings);

	if (AK::MusicEngine::Init(&musicSettings) != AK_Success)
	{
		DEBUG_LOG("Couldn't initialize the music engine");
		return false;
	}

	// Initializing the spatial audio
	AkSpatialAudioInitSettings spatialSettings;

	if (AK::SpatialAudio::Init(spatialSettings) != AK_Success)
	{
		DEBUG_LOG("Couldn't initialize the spatial audio");
		return false;
	}

	// Initializing communication. Only needed in DEBUG mode
#ifndef AK_OPTIMIZED

	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);

	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		DEBUG_LOG("Couldn't initialize communication");
		return false;
	}

#endif

	lowLevelIO.SetBasePath(AKTEXT("Assets/Wwise/"));

	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

	ReadIDs();

	AkBankID bankID = 0;
	for (int i = 0; i < wwiseInfo.banks.size(); ++i)
	{
		std::string name = wwiseInfo.banks[i] + ".bnk";
		if (AK::SoundEngine::LoadBank(name.c_str(), bankID) != AK_Success)
		{
			DEBUG_LOG("Couldn't find the bank: %s", name.c_str());
			return false;
		}
	}

	return true;
}

bool AudioManager::Update()
{
	return true;
}

void AudioManager::Render()
{
	AK::SoundEngine::RenderAudio();
}

void AudioManager::RegisterGameObject(int uuid)
{
	AK::SoundEngine::RegisterGameObj(uuid);
}

void AudioManager::UnregisterGameObject(int uuid)
{
	AK::SoundEngine::UnregisterGameObj(uuid);
}

void AudioManager::AddReverbZone(AudioReverbZoneComponent* reverbZone)
{
	reverbZones.push_back(reverbZone);
}

void AudioManager::DeleteReverbZone(AudioReverbZoneComponent* reverbZone)
{
	std::vector<AudioReverbZoneComponent*>::iterator iterator = reverbZones.begin();

	for (; iterator != reverbZones.end(); ++iterator)
	{
		if (*iterator == reverbZone)
		{
			reverbZones.erase(iterator);
			break;
		}
	}
}

void AudioManager::StopAllAudioSources()
{
	AK::SoundEngine::StopAll();
}

void AudioManager::PlayAllAudioSources()
{
	for (int i = 0; i < audioSources.size(); ++i)
	{
		audioSources[i]->PlayClipOnAwake();
	}
}

void AudioManager::ResumeAllAudioSources()
{
	for (int i = 0; i < audioSources.size(); ++i)
	{
		audioSources[i]->ResumeClip();
	}
}

void AudioManager::PauseAllAudioSources()
{
	for (int i = 0; i < audioSources.size(); ++i)
	{
		audioSources[i]->PauseClip();
	}
}

void AudioManager::CheckReverbGameObject(unsigned int UUID)
{
	AkAuxSendValue aEnvs;
	for (int i = 0; i < reverbZones.size(); ++i)
	{
		if (reverbZones[i]->GetReverbZoneAABB().Contains(currentListenerPosition->GetPosition()))
		{
			aEnvs.listenerID = AK_INVALID_GAME_OBJECT;
			aEnvs.auxBusID = AK::SoundEngine::GetIDFromString(reverbZones[i]->GetReverbBusName().c_str());
			aEnvs.fControlValue = 1.5f;

			if (AK::SoundEngine::SetGameObjectAuxSendValues(UUID, &aEnvs, 1) != AK_Success)
			{
				DEBUG_LOG("Couldnt set aux send values");
			}
		}
		else
		{
			aEnvs.listenerID = AK_INVALID_GAME_OBJECT;
			aEnvs.auxBusID = AK::SoundEngine::GetIDFromString(L"Master Audio Bus");
			aEnvs.fControlValue = 1.0f;

			if (AK::SoundEngine::SetGameObjectAuxSendValues(UUID, &aEnvs, 1) != AK_Success)
			{
				DEBUG_LOG("Couldnt set aux send values");
			}
		}
	}
}

void AudioManager::AddAudioSource(AudioSourceComponent* audioSource)
{
	audioSources.push_back(audioSource);
}

void AudioManager::DeleteAudioSource(AudioSourceComponent* audioSource)
{
	std::vector<AudioSourceComponent*>::iterator iterator = audioSources.begin();

	for (; iterator != audioSources.end(); ++iterator)
	{
		if (*iterator == audioSource)
		{
			audioSources.erase(iterator);
			break;
		}
	}
}

void AudioManager::SetDefaultListener(AkGameObjectID* uuid, TransformComponent* listenerPosition)
{
	AK::SoundEngine::SetDefaultListeners(uuid, 1);
	currentListenerPosition = listenerPosition;
}

void AudioManager::SetPosition(int uuid, AkSoundPosition position)
{
	AK::SoundEngine::SetPosition(uuid, position);
}

AkPlayingID AudioManager::PostEvent(const char* name, int uuid)
{
	AkPlayingID playingID = AK::SoundEngine::PostEvent(name, uuid);
	if (playingID == AK_INVALID_PLAYING_ID)
	{
		DEBUG_LOG("Post event %s failed", name);
		return -1;
	}

	return playingID;
}

AudioManager::AudioManager() : currentListenerPosition(nullptr)
{
}

void AudioManager::ReadIDs()
{
	std::ifstream file("Assets/Wwise/Wwise_IDs.h");
	
	std::string line;

	while (std::getline(file, line))
	{
		if (line.find("EVENTS") != std::string::npos)
		{
			while (std::getline(file, line))
			{
				if (line.find("}") != std::string::npos)
				{
					break;
				}
				else if(line.find("AkUniqueID") != std::string::npos)
				{
					line = line.substr(0, line.find("=") - 1);
					line = line.substr(line.find_last_of(" ") + 1, line.length());

					wwiseInfo.events.push_back(line);
				}
			}
		}
		else if (line.find("BANKS") != std::string::npos)
		{
			while (std::getline(file, line))
			{
				if (line.find("}") != std::string::npos)
				{
					break;
				}
				else if (line.find("AkUniqueID") != std::string::npos)
				{
					line = line.substr(0, line.find("=") - 1);
					line = line.substr(line.find_last_of(" ") + 1, line.length());

					wwiseInfo.banks.push_back(line);
				}
			}
		}
		else if (line.find(" BUSSES") != std::string::npos)
		{
			while (std::getline(file, line))
			{
				if (line.find("}") != std::string::npos)
				{
					break;
				}
				else if (line.find("AkUniqueID") != std::string::npos)
				{
					line = line.substr(0, line.find("=") - 1);
					line = line.substr(line.find_last_of(" ") + 1, line.length());

					wwiseInfo.busses.push_back(line);
				}
			}
		}
		else if (line.find("AUX_BUSSES") != std::string::npos)
		{
			while (std::getline(file, line))
			{
				if (line.find("}") != std::string::npos)
				{
					break;
				}
				else if (line.find("AkUniqueID") != std::string::npos)
				{
					line = line.substr(0, line.find("=") - 1);
					line = line.substr(line.find_last_of(" ") + 1, line.length());

					wwiseInfo.auxBusses.push_back(line);
				}
			}
		}
	}
}