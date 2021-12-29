#pragma once

#include "Component.h"
#include "MathGeoLib/src/Geometry/AABB.h"
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

	void ResizeReverbZone();

	inline AABB GetReverbZoneAABB() const { return reverbBoxZone; }
	inline std::string GetReverbBusName() const { return busReverb; }

	void CompileBuffers();
	void Draw() override;

	bool Update(float dt) override;

	void OnEditor() override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

private:
	TransformComponent* transform;

	AABB reverbBoxZone;
	std::string busReverb;

	// Show reverb zone for debug purposes
	IndexBuffer* ebo;
	VertexBuffer* vbo;

	float3 dimensions;
};