#pragma once

#include "Component.h"
#include "MathGeoLib/src/Geometry/Frustum.h"

class TransformComponent;
class IndexBuffer;
class VertexBuffer;
class CameraComponent : public Component
{
public:
	CameraComponent(GameObject* own, TransformComponent* trans);
	~CameraComponent();

	void OnEditor() override;
	bool Update(float dt) override;
	void Draw() override;

	void SetPlanes();
	void SetFov();
	
	void CompileBuffers();
	bool CompareRotations(Quat& quat1, Quat& quat2);

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	float4x4 matrixViewFrustum;
	float4x4 matrixProjectionFrustum;
private:

	Frustum camera;
	TransformComponent* transform;
	Quat currentRotation;
	float3 currentPos;

	float nearPlane;
	float farPlane;
	float verticalFov;
	float horizontalFov;

	IndexBuffer* ebo;
	VertexBuffer* vbo;

};