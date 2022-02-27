#pragma once

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"
#include "Primitive.h"
#include "Bone.h"
#include <memory>

class BoneComponent : public Component
{
public:
	BoneComponent(GameObject* own);
	BoneComponent(BoneComponent* cBone);
	~BoneComponent();

	bool Update(float dt) override;
	void DebugDraw();
	void OnEditor() override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void SetBone(std::shared_ptr<Resource> b);

	void SetDebugDraw();

public:

	bool debugDraw = false;

	float4x4 globalOffset = float4x4::identity;

	std::shared_ptr<Bone2> bone;
};