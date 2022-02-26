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

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void SetAnimation(std::shared_ptr<Resource> a);

public:
	bool showAnimMenu = false;

	bool loop = false;
	bool blendLoop = false;
	bool debugDraw = false;
	bool bonesLoaded = false;
	bool blend = false;
	bool smoothT = false;
	bool frozenT = false;
	bool interpolation = false;
	bool TestPlay = false;
	bool TestPause = false;

	uint blendRUID = 0;

	uint idleUID = 0;
	uint walkUID = 0;
	uint attackUID = 0;

	float walkBlendTime = 0.1f;
	float attackBlendTime = 0.1f;

	float animTime = 0.0f;
	float blendAnimTime = 0.0f;
	float blendPercent = 0.0f;
	float blendTime = 0.0f;
	float totalBlendTime = 0.1f;
	float speed = 1.0f;

	//bone transformation index and bone GameObject UID
	std::map<uint, uint> bones;
	std::map<uint, uint> blendBones;

	std::shared_ptr<Animation> anim;
	std::shared_ptr<Animation> anim1;
	std::shared_ptr<Animation> anim2;
};