#pragma once

#include <AK/SoundEngine/Win32/AkFilePackageLowLevelIOBlocking.h>
#include "AudioReverbZoneComponent.h"
#include <vector>

class TransformComponent;

class AudioManager
{
public:
	static void Create();
	static void Release();
	
	~AudioManager(){}
	
	static inline AudioManager* Get() { return instance; }

	bool Init();

	bool Update();

	void Render();

	void RegisterGameObject(int uuid);
	void UnregisterGameObject(int uuid);

	void AddReverbZone(AudioReverbZoneComponent* reverbZone);
	void DeleteReverbZone(AudioReverbZoneComponent* reverbZone);
	void CheckReverbGameObject(unsigned int UUID);

	void SetDefaultListener(AkGameObjectID* uuid, TransformComponent* listenerPosition);
	void SetPosition(int uuid, AkSoundPosition position);

	void PostEvent(const char* name, int uuid);

private:
	AudioManager();

	// List to have all reverb zones in this singleton
	std::vector<AudioReverbZoneComponent*> reverbZones;

	TransformComponent* currentListenerPosition;

	static AudioManager* instance;
	CAkFilePackageLowLevelIOBlocking lowLevelIO;
};