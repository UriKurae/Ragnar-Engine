#pragma once
#include "Component.h"
#include "Geometry/AABB.h"
#include <string>

class GameObject;
class TransformComponent;
class IndexBuffer;
class VertexBuffer;

class AudioReverbZoneComponent : public Component
{
public:
	AudioReverbZoneComponent(GameObject* own, TransformComponent* trans);
	~AudioReverbZoneComponent();

	bool Update(float dt) override;
	void Draw(CameraComponent* gameCam = nullptr) override;
	void OnEditor() override;

	void ResizeReverbZone();
	void CompileBuffers();

	inline AABB GetReverbZoneAABB() const { return reverbBoxZone; }
	inline std::string GetReverbBusName() const { return busReverb; }

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	inline void ChangePosition() { changePosition = true; }

private:
	bool changePosition;
	TransformComponent* transform;

	AABB reverbBoxZone;
	std::string busReverb;

	// Show reverb zone for debug purposes
	IndexBuffer* ebo;
	VertexBuffer* vbo;

	float3 dimensions;
};