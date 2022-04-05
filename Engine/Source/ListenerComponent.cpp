#include "ListenerComponent.h"
#include "AudioManager.h"

#include "GameObject.h"
#include "TransformComponent.h"

ListenerComponent::ListenerComponent(GameObject* own, TransformComponent* trans) : changePosition(true), activeListener(true), transform(trans)
{
	owner = own;
	type = ComponentType::AUDIO_LISTENER;
	
	// Register this listener
	AkGameObjectID cameraID = owner->GetUUID();
	if (!owner->CheckAudioRegister())
	{
		AudioManager::Get()->RegisterGameObject(cameraID);
		owner->SetAudioRegister(true);
	}
	
	AudioManager::Get()->SetDefaultListener(&cameraID, owner->GetComponent<TransformComponent>());
}

ListenerComponent::~ListenerComponent()
{
	AudioManager::Get()->UnregisterGameObject(owner->GetUUID());
}

void ListenerComponent::OnEditor()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader(ICON_FA_HEADPHONES" Listener"))
	{
		ImGui::Text("AudioClip");
		ImGui::SameLine(ImGui::GetWindowWidth() * 0.65f);
		ImGui::Checkbox("##AudioClip", &activeListener);
		ImGui::SameLine();
		ImGui::Text("Listen");

		ComponentOptions(this);
		ImGui::Separator();
	}
	ImGui::PopID();
}

bool ListenerComponent::Update(float dt)
{
	if (changePosition)
	{
		float3 position = transform->GetPosition();
		AkSoundPosition audioSourcePos;
		float3 orientation = transform->GetRotation().ToEulerXYZ().Normalized();
		audioSourcePos.SetOrientation({ 0,0,-1 }, { 0,1,0 });
		audioSourcePos.SetPosition(position.x, position.y, position.z);
		AudioManager::Get()->SetPosition(owner->GetUUID(), audioSourcePos);

		changePosition = false;
	}

	return true;
}

bool ListenerComponent::OnLoad(JsonParsing& node)
{
	// Register this audio source
	if (!owner->CheckAudioRegister())
	{
		AkGameObjectID cameraID = owner->GetUUID();
		AudioManager::Get()->RegisterGameObject(cameraID);
		owner->SetAudioRegister(true);
	}
	activeListener = node.GetJsonBool("Active");

	return true;
}

bool ListenerComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", activeListener);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}