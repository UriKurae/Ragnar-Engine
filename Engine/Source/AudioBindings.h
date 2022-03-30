#pragma once

#include "GameObject.h"
#include "AudioSourceComponent.h"
#include "ListenerComponent.h"

#include "ScriptBindings.h"

#include <metadata\object-forward.h>
#include <metadata\object.h>
#include <metadata/class.h>

// AUDIO SOURCE ================================================
void PlayClip(MonoObject* clipName, MonoObject* owner)
{
	// IMPORTANT! Names are flipped but it works, if i try to flip them it won't work
	char* name = mono_string_to_utf8(mono_object_to_string(owner, 0));
	GetComponentMono<AudioSourceComponent*>(clipName)->PlayClip(name);
}

void StopCurrentClip(MonoObject* go)
{
	GetComponentMono<AudioSourceComponent*>(go)->StopClip();
}

void SetClipVolume(MonoObject* go, float vol)
{
	GetComponentMono<AudioSourceComponent*>(go)->SetClipVolume(vol);
}

// AUDIO LISTENER ============================================
void TestListener(MonoObject* go)
{
	GetComponentMono<ListenerComponent*>(go);
}
