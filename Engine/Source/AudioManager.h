#pragma once

#include <AK/SoundEngine/Win32/AkFilePackageLowLevelIOBlocking.h>

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

	void SetDefaultListener(AkGameObjectID* uuid);
	void SetPosition(int uuid, AkSoundPosition position);

	void PostEvent(const char* name, int uuid);

private:
	AudioManager();

	static AudioManager* instance;
	CAkFilePackageLowLevelIOBlocking lowLevelIO;
};