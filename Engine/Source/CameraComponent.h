#pragma once

#include "Component.h"
#include "MathGeoLib/src/Geometry/Frustum.h"

class CameraComponent : public Component
{
public:
	CameraComponent(GameObject* own);
	~CameraComponent();

	void OnEditor() override;
	bool Update(float dt) override;

	void SetPlanes();
	void SetFov();

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

private:

	Frustum camera;

	float nearPlane;
	float farPlane;
	float verticalFov;
	float horizontalFov;
};