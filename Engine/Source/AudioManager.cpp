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

	AkBankID bankID = 0;
	if (AK::SoundEngine::LoadBank(L"Init.bnk", bankID) != AK_Success)
	{
		DEBUG_LOG("Couldn't find the bank: Init.bnk");
		return false;
	}
	if (AK::SoundEngine::LoadBank(L"ragnarEngine.bnk", bankID) != AK_Success)
	{
		DEBUG_LOG("Couldn't find the bank: ragnarEngine.bnk");
		return false;
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

void AudioManager::SetDefaultListener(AkGameObjectID* uuid, TransformComponent* listenerPosition)
{
	AK::SoundEngine::SetDefaultListeners(uuid, 1);
	currentListenerPosition = listenerPosition;
}

void AudioManager::SetPosition(int uuid, AkSoundPosition position)
{
	AK::SoundEngine::SetPosition(uuid, position);
}

void AudioManager::PostEvent(const char* name, int uuid)
{
	if (AK::SoundEngine::PostEvent(name, uuid) == AK_INVALID_PLAYING_ID)
	{
		DEBUG_LOG("Post event %s failed", name);
	}
}

AudioManager::AudioManager()
{
}