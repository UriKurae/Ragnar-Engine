#include "Globals.h"
#include "ListenerComponent.h"
#include "GameObject.h"
#include "AudioManager.h"
#include "TransformComponent.h"

#include "Imgui/imgui_internal.h"

ListenerComponent::ListenerComponent(GameObject* own, TransformComponent* trans) : activeListener(true), transform(trans)
{
	owner = own;
	type = ComponentType::AUDIO_LISTENER;
	
	// Register this listener
	if (!owner->CheckAudioRegister())
	{
		AkGameObjectID cameraID = owner->GetUUID();
		AudioManager::Get()->RegisterGameObject(cameraID);
		owner->SetAudioRegister(true);
	}
}

ListenerComponent::~ListenerComponent()
{
	AudioManager::Get()->UnregisterGameObject(owner->GetUUID());
}

void ListenerComponent::OnEditor()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Listener"))
	{
		ImGui::Text("AudioClip");
		ImGui::SameLine(ImGui::GetWindowWidth() * 0.65f);
		ImGui::Checkbox("##AudioClip", &activeListener);
		ImGui::SameLine();
		ImGui::Text("Listen");
	}
	ImGui::PopID();
}

bool ListenerComponent::Update(float dt)
{
	float3 position = transform->GetPosition();
	AkSoundPosition audioSourcePos;
	float3 orientation = transform->GetRotation().ToEulerXYZ().Normalized();
	//audioSourcePos.SetOrientation({ orientation.x, orientation.y, orientation.z }, { orientation.x, orientation.y, orientation.z });
	audioSourcePos.SetOrientation({ 0,0,-1 }, {0,1,0 });
	audioSourcePos.SetPosition(position.x, position.y, position.z);
	AudioManager::Get()->SetPosition(owner->GetUUID(), audioSourcePos);
	//DEBUG_LOG("Source: x %f, y %f, z %f", position.x, position.y, position.z);

	return true;
}

bool ListenerComponent::OnLoad(JsonParsing& node)
{
	return true;
}

bool ListenerComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	return true;
}