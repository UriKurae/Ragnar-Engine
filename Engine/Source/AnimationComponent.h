#pragma once
#include "Component.h"
#include "Animation.h"

#include <map>
#include <memory>
#include <queue>


struct AnimState
{
	std::string state;
	std::shared_ptr<Animation> anim;
	bool loop;
	bool hasFinished;
};

class AnimationComponent : public Component
{
public: 
	AnimationComponent(GameObject* own);
	AnimationComponent(AnimationComponent* animation);
	~AnimationComponent();

	void OnEditor() override;

	bool Update(float dt) override;

	void CalculateBoneTransform(HierarchyData& data, HierarchyData& lastData, float4x4 parentTransform);

	float4x4 InterpolateWithoutBones(float4x4& transform, float4x4& lastTransform);
	float4x4 InterpolateWithOneBone(float4x4& transform, Bone& bone);
	float4x4 InterpolateWithOneBone(Bone& bone, float4x4& transform);

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void SetAnimation(std::shared_ptr<Resource> a);
	void Play(std::string state);
	
	void GetAnimations();

	std::vector<float4x4> GetFinalBoneMatrices()
	{
		return finalBoneMatrices;
	}

public:
	bool showAnimMenu = false;
	bool playing;

	bool interpolating;

	float lastCurrentTime;
	float currentTime;
	float loopTime;
	float deltaTime;

	std::vector<float4x4> finalBoneMatrices;

	AnimState* lastAnim;
	AnimState* currAnim;
	std::vector<AnimState> animations;

	std::queue<AnimState*> animQueue;
};