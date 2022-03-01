#pragma once

#include "Component.h"
#include "Globals.h"
#include "Animation.h"
#include <map>
#include <memory>

class AnimationComponent : public Component
{
public: 
	AnimationComponent(GameObject* own);
	AnimationComponent(AnimationComponent* animation);
	~AnimationComponent();

	void OnEditor() override;

	bool Update(float dt) override;

	void CalculateBoneTransform(/*const AssimpNodeData* node,*/ float4x4 parentTransform);

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void SetAnimation(std::shared_ptr<Resource> a);
	
	void GetAnimations();

	std::vector<float4x4> GetFinalBoneMatrices()
	{
		return finalBoneMatrices;
	}

public:
	bool showAnimMenu = false;

	float currentTime;
	float deltaTime;

	std::vector<float4x4> finalBoneMatrices;

	std::shared_ptr<Animation> anim;
};