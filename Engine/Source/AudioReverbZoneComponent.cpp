#include "Globals.h"
#include "AudioReverbZoneComponent.h"

#include "GameObject.h"
#include "AudioManager.h"
#include <GL\glew.h>

AudioReverbZoneComponent::AudioReverbZoneComponent(GameObject* own, TransformComponent* trans) : transform(trans), Component(), busReverb("None"), vbo(nullptr), ebo(nullptr), dimensions(5.0f, 5.0f, 5.0f)
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
	ResizeReverbZone();
	CompileBuffers();
	AudioManager::Get()->AddReverbZone(this);
}

AudioReverbZoneComponent::~AudioReverbZoneComponent()
{
	AudioManager::Get()->DeleteReverbZone(this);
	RELEASE(vbo);
	RELEASE(ebo);
}

void AudioReverbZoneComponent::ResizeReverbZone()
{
	reverbBoxZone.SetNegativeInfinity();	
	reverbBoxZone.SetFromCenterAndSize(transform->GetPosition(), dimensions);
}

void AudioReverbZoneComponent::CompileBuffers()
{
	// Configure buffers
	float3 corners[8];
	reverbBoxZone.GetCornerPoints(corners);
	unsigned int indices[24] =
	{
		0,1,
		1,3,
		3,2,
		2,0,

		1,5,
		4,6,
		7,3,

		6,7,
		6,2,

		7,5,
		4,5,

		4,0
	};

	if (vbo)
	{
		vbo->Unbind();
		RELEASE(vbo);
	}
	ebo = new IndexBuffer(indices, 24);
	vbo = new VertexBuffer(corners, sizeof(float3) * 8);
	ebo->Unbind();
	vbo->Unbind();
}

void AudioReverbZoneComponent::Draw()
{
	glPushMatrix();

	glMultMatrixf(transform->GetGlobalTransform().Transposed().ptr());
	glEnableClientState(GL_VERTEX_ARRAY);
	vbo->Bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	ebo->Bind();
	glLineWidth(2.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glDrawElements(GL_LINES, ebo->GetSize(), GL_UNSIGNED_INT, NULL);
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);
	vbo->Unbind();
	ebo->Unbind();
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
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
		ImGui::Text("Reverb effect");
		ImGui::SameLine();
		if (ImGui::BeginCombo("##Audio Reverb Zone", busReverb.c_str()))
		{
			if (ImGui::Selectable("None"))
			{
				busReverb = "None";
			}
			std::vector<std::string> busses = AudioManager::Get()->GetBussesList();
			for (int i = 0; i < busses.size(); ++i)
			{
				if (ImGui::Selectable(busses[i].c_str()))
				{
					busReverb = busses[i];
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::SliderFloat3("##Dimensions", &dimensions[0], 0, 20))
		{
			ResizeReverbZone();
			CompileBuffers();
		}
	}
}

bool AudioReverbZoneComponent::OnLoad(JsonParsing& node)
{
	busReverb = node.GetJsonString("Reverb Zone");
	dimensions = node.GetJson3Number(node, "Dimensions");

	ResizeReverbZone();
	CompileBuffers();

	return false;
}

bool AudioReverbZoneComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Reverb Zone", busReverb.c_str());
	file.SetNewJson3Number(file, "Dimensions", dimensions);

	node.SetValueToArray(array, file.GetRootValue());

	return false;
}