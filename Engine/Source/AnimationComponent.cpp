#include "Animation.h"
#include "AnimationComponent.h"
#include "Application.h"
#include "ModuleSceneManager.h"
#include "Scene.h"

#include "MeshComponent.h"

#include "FileSystem.h"
#include "ResourceManager.h"

#include "Math/TransformOps.h"
#include "Imgui/imgui_stdlib.h"

#include "Profiling.h"

AnimationComponent::AnimationComponent(GameObject* own) : showAnimMenu(false), deltaTime(0.0f), currAnim(nullptr), playing(false), loopTime(0.0f), interpolating(false), lastAnim(nullptr), lastCurrentTime(0.0f), interpolatingVel(1.0f)
{
	type = ComponentType::ANIMATION;
	owner = own;

	currentTime = 0.0f;

	finalBoneMatrices.reserve(100);

	for (int i = 0; i < 100; i++)
	{
		finalBoneMatrices.push_back(float4x4::identity);
	}

	if (MeshComponent* mesh = owner->GetComponent<MeshComponent>())
	{
		boneInfoMap = mesh->GetBoneMap();
	}

	animations.push_back({ "None", nullptr, false });

	active = true;
}

AnimationComponent::AnimationComponent(AnimationComponent* animation) : showAnimMenu(false)
{
	currAnim = animation->currAnim;
}

AnimationComponent::~AnimationComponent()
{
	for (int i = 0; i < animations.size(); ++i)
	{
		std::shared_ptr<Animation> anim = animations[i].anim;
		if (anim.use_count() - 1 == 1)
		{
			anim->UnLoad();
		}
	}
}

void AnimationComponent::OnEditor()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader(ICON_FA_PEOPLE_ARROWS" Animation Component"))
	{
		for (int i = 0; i < animations.size(); ++i)
		{
			AnimState* currState = &animations[i];
			ImGui::PushID((void*)currState->state.c_str());

			int width = ImGui::GetWindowContentRegionMax().x;
			ImGui::PushItemWidth(width / 3);
			ImGui::InputText("##State", &currState->state, 64);
			ImGui::PopItemWidth();

			unsigned int test = currState->state.size();
			ImGui::SameLine();

			ImGui::PushItemWidth(2 * width / 3 - 15.0f);
			if (ImGui::BeginCombo("##Animations", currState->anim ? currState->anim->GetName().c_str() : "None"))
			{
				if (ImGui::Selectable("None"))
				{
					currState->state = "None";
					currState->anim = nullptr;
					currState->loop = false;
				}

				std::vector<std::string> files;
				app->fs->DiscoverFiles("Library/Animations", files);
				for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
				{
					if ((*it).find(".rganim") != std::string::npos)
					{
						ImGui::PushID((*it).c_str());
						app->fs->GetFilenameWithoutExtension(*it);
						*it = (*it).substr((*it).find_last_of("_") + 1, (*it).length());
						uint uid = std::stoll(*it);
						std::shared_ptr<Animation> res = std::static_pointer_cast<Animation>(ResourceManager::GetInstance()->GetResource(uid));
						if (ImGui::Selectable(res->GetName().c_str()))
						{
							res->Load();
							currState->anim = res;
							if (currAnim && currAnim->anim.use_count() - 1 == 1) currAnim->anim->UnLoad();
							currAnim = currState;
						}
						ImGui::PopID();
					}
				}

				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			ImGui::Checkbox("Loop", &currState->loop);
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_INFO) && currAnim)
			{
				currAnim = currState;
			}
			ImGui::PopID();
		}

		if (ImGui::Button(ICON_FA_PLUS))
		{
			if (currAnim != nullptr)
			{
				std::string aux = currAnim->state;
				animations.push_back({ "None", nullptr, false, false });
				for (int i = 0; i < animations.size(); ++i)
				{
					if (animations[i].state == aux) currAnim = &animations[i];
				}
			}
			else
			{
				animations.push_back({ "None", nullptr, false, false });
			}
		}
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_MINUS))
		{
			animations.erase(animations.end() - 1);
		}

		ImGui::DragFloat("Interp. Velocity", &interpolatingVel, 0.5f, 1.0f);
		ImGui::Separator();

		AnimationInfo();
	}
	ImGui::PopID();
}

void AnimationComponent::AnimationInfo()
{
	if (currAnim && currAnim->anim)
	{
		ImGui::Text("Path: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", currAnim->anim->GetAssetsPath().c_str());
		ImGui::Text("Ticks: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.0f", currAnim->anim->GetTicks());
		ImGui::Text("Ticks Per Second: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.0f", currAnim->anim->GetTicksPerSecond());
		ImGui::Text("Duration: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.0f s", currAnim->anim->GetDuration());
		ImGui::Text("Bones Attached: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d bones", currAnim->anim->bones.size());
	}
	else
	{
		ImGui::Text("There's is no current animation");
	}
}

bool AnimationComponent::Update(float dt)
{
	RG_PROFILING_FUNCTION("Animation Component Update");

	deltaTime = dt;
	if (currAnim && playing && app->sceneManager->GetGameState() == GameState::PLAYING)
	{
		// Loop time keeps track of the miliseconds that passed since the start of the animation
		// GetDuration gets the duration of the animation in miliseconds
		if (loopTime > currAnim->anim->GetDuration())
		{
			currAnim->hasFinished = true;
			// When it reaches the desired duration, we reset everything
			if (currAnim->loop && animQueue.empty())
			{
				loopTime = 0.0f;
			}
			else
			{
				loopTime = 0.0f;
				playing = false;
				lastAnim = currAnim;

				if (!animQueue.empty())
				{
					currAnim = animQueue.front();
					animQueue.pop();
				}

				if (lastAnim != currAnim) interpolating = true;
			}
		}

		if (!interpolating)
		{
			// Loop time increases by our delta time
			loopTime += dt;
			currentTime += currAnim->anim->GetTicksPerSecond() * dt;
			currentTime = fmod(currentTime, currAnim->anim->GetTicks());
		}
		else
		{
			loopTime += dt;
			// 60 is the frames per second that we want
			currentTime += 24.0f * dt;
			currentTime = fmod(currentTime, currAnim->anim->GetTicks());
		}
		//auto& boneInfoMap = owner->GetComponent<MeshComponent>()->GetBoneMap();
		CalculateBoneTransform(currAnim->anim->GetHierarchyData(), float4x4::identity);
		//CalculateBoneTransform(currAnim->anim->GetBones(), float4x4::identity);
	}

	return true;
}

void AnimationComponent::CalculateBoneTransform(HierarchyData& data, float4x4 parentTransform)
{
	RG_PROFILING_FUNCTION("Calculate Bone Transform");

	std::string nodeName = data.name;

	float4x4 globalTransformation = float4x4::identity;

	if (GetBoneInfo(nodeName, boneInfoMap) != boneInfoMap.end())
	{
		Bone* bone = currAnim->anim->FindBone(nodeName);
		Bone* lastBone = lastAnim->anim->FindBone(nodeName);

		if (bone)
		{
			globalTransformation = UpdateBone(bone, lastBone, parentTransform);

			const BoneInfo& boneInfo = boneInfoMap.at(nodeName);
			int index = boneInfo.id;
			const float4x4& offset = boneInfo.offset;
			finalBoneMatrices[index] = globalTransformation * offset;
		}
	}

	for (int i = 0; i < data.childrenCount; ++i)
		CalculateBoneTransform(data.children[i], globalTransformation);
}

void AnimationComponent::CalculateBoneTransform(std::vector<Bone>& bones, float4x4 parentTransform)
{
	float4x4 nodeTransform = bones[0].GetTransform();

	std::vector<Bone> currentBones = currAnim->anim->GetBones();
	std::vector<Bone> lastBones = currAnim->anim->GetBones();

	if (!interpolating)
	{
		for (int i = 0; i < currentBones.size(); ++i)
		{
			Bone* currBone = &currentBones[i];
			if (currBone)
			{
				currBone->Update(currentTime);
				nodeTransform = currBone->GetTransform();
			}
			float4x4 globalTransformation;
			if (currentBones[i].GetParentId() != -1) globalTransformation = currentBones[currentBones[i].GetParentId()].GetTransform() * currBone->GetTransform();
			else
				currBone->GetTransform();

			auto boneInfoMap = owner->GetComponent<MeshComponent>()->GetBoneMap();
			if (boneInfoMap.find(currentBones[i].GetName()) != boneInfoMap.end())
			{
				int index = boneInfoMap[currentBones[i].GetName()].id;
				float4x4 offset = boneInfoMap[currentBones[i].GetName()].offset;
				finalBoneMatrices[index] = globalTransformation * offset;
			}
		}
	}
	else if (lastAnim != currAnim)
	{
		for (int i = 0; i < currentBones.size(); ++i)
		{
			Bone* currBone = &currentBones[i];
			Bone* lastBone = &lastBones[i];
			if (currBone && lastBone)
			{
				currBone->UpdateInterpolation(*lastBone, currentTime, lastCurrentTime, interpolating, interpolatingVel);
				nodeTransform = currBone->GetTransform();
			}
			float4x4 globalTransformation;
			if (currentBones[i].GetParentId() != -1) globalTransformation = currentBones[currentBones[i].GetParentId()].GetTransform() * currBone->GetTransform();
			else
				currBone->GetTransform();

			auto boneInfoMap = owner->GetComponent<MeshComponent>()->GetBoneMap();
			if (boneInfoMap.find(currentBones[i].GetName()) != boneInfoMap.end())
			{
				int index = boneInfoMap[currentBones[i].GetName()].id;
				float4x4 offset = boneInfoMap[currentBones[i].GetName()].offset;
				finalBoneMatrices[index] = globalTransformation * offset;
			}
		}
	}
}

float4x4 AnimationComponent::InterpolateWithoutBones(float4x4& transform, float4x4& lastTransform)
{
	float3 lastPosition;
	Quat lastRotation;
	float3 lastScale;

	lastTransform.Decompose(lastPosition, lastRotation, lastScale);

	float3 position;
	Quat rotation;
	float3 scale;

	transform.Decompose(position, rotation, scale);

	float scaleFactor = 0.0f;
	float midWayLength = currentTime - 0;
	float framesDiff = 60 - 0;
	scaleFactor = midWayLength / framesDiff;
	if (scaleFactor < 0) scaleFactor = 0.0f;

	float3 finalPos = Lerp(lastPosition, position, scaleFactor);

	float4x4 pos = float4x4::Translate(finalPos);
	float4x4 rot = float4x4(Slerp(lastRotation, rotation, scaleFactor));
	float4x4 sca = float4x4::Scale(Lerp(lastScale, scale, scaleFactor));

	if (scaleFactor >= 1.0f)
		interpolating = false;

	return pos * rot * sca;
}

float4x4 AnimationComponent::InterpolateWithOneBone(float4x4& transform, Bone& bone)
{
	float3 position;
	Quat rotation;
	float3 scale;

	transform.Decompose(position, rotation, scale);

	float scaleFactor = 0.0f;
	float midWayLength = currentTime - 0;
	float framesDiff = 60 - 0;
	scaleFactor = midWayLength / framesDiff;
	if (scaleFactor < 0) scaleFactor = 0.0f;

	float3 p;
	Quat r;
	float3 s;

	bone.GetData().keyFrames[0].matrix.Decompose(p, r, s);

	float3 finalPos = Lerp(position, p, scaleFactor);

	float4x4 pos = float4x4::Translate(finalPos);
	float4x4 rot = float4x4(Slerp(rotation, r, scaleFactor));
	float4x4 sca = float4x4::Scale(Lerp(scale, s, scaleFactor));

	if (scaleFactor >= 1.0f)
		interpolating = false;

	return pos * rot * sca;
}

float4x4 AnimationComponent::InterpolateWithOneBone(Bone& bone, float4x4& transform)
{
	float3 position;
	Quat rotation;
	float3 scale;

	transform.Decompose(position, rotation, scale);

	int posIndex = bone.GetPositionIndex(lastCurrentTime);
	int rotIndex = bone.GetRotationIndex(lastCurrentTime);
	int scaIndex = bone.GetScalingIndex(lastCurrentTime);

	float scaleFactor = 0.0f;
	float midWayLength = currentTime - 0;
	float framesDiff = 60 - 0;
	scaleFactor = midWayLength / framesDiff;
	if (scaleFactor < 0) scaleFactor = 0.0f;

	float3 p;
	Quat r;
	float3 s;

	bone.GetData().keyFrames[0].matrix.Decompose(p, r, s);

	float3 finalPos = Lerp(position, p, scaleFactor);

	float4x4 pos = float4x4::Translate(finalPos);
	float4x4 rot = float4x4(Slerp(rotation, r, scaleFactor));
	float4x4 sca = float4x4::Scale(Lerp(scale, s, scaleFactor));

	if (scaleFactor >= 1.0f)
		interpolating = false;

	return pos * rot * sca;
}

bool AnimationComponent::OnLoad(JsonParsing& node)
{
	active = node.GetJsonBool("Active");

	JSON_Array* jsonArray = node.GetJsonArray(node.ValueToObject(node.GetRootValue()), "Animations");

	animations.clear();

	std::string currAnimState = node.GetJsonString("Current Anim");
	size_t size = node.GetJsonArrayCount(jsonArray);
	for (int i = 0; i < size; ++i)
	{
		AnimState anim;

		JsonParsing c = node.GetJsonArrayValue(jsonArray, i);
		anim.state = c.GetJsonString("State");
		anim.loop = c.GetJsonBool("Loop");
		anim.anim = std::static_pointer_cast<Animation>(ResourceManager::GetInstance()->LoadResource(std::string(c.GetJsonString("Path Anim Assets"))));

		animations.push_back(anim);
	}

	for (std::vector<AnimState>::iterator it = animations.begin(); it != animations.end(); ++it)
	{
		if (currAnimState == (*it).state)
		{
			currAnim = &(*it);
			break;
		}
	}

	return true;
}

bool AnimationComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);

	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Current Anim", currAnim->state.c_str());

	JSON_Array* newArray = file.SetNewJsonArray(file.GetRootValue(), "Animations");
	for (std::vector<AnimState>::iterator it = animations.begin(); it != animations.end(); ++it)
	{
		JsonParsing animF = JsonParsing();

		animF.SetNewJsonString(animF.ValueToObject(animF.GetRootValue()), "State", (*it).state.c_str());
		animF.SetNewJsonString(animF.ValueToObject(animF.GetRootValue()), "Path Anim Assets", (*it).anim != nullptr ? (*it).anim.get()->GetAssetsPath().c_str() : "");
		animF.SetNewJsonBool(animF.ValueToObject(animF.GetRootValue()), "Loop", (*it).loop);

		file.SetValueToArray(newArray, animF.GetRootValue());
	}

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void AnimationComponent::SetAnimation(std::shared_ptr<Resource> a)
{
	//currAnim = std::static_pointer_cast<Animation>(a);
}

void AnimationComponent::Play(std::string state)
{
	if (app->sceneManager->GetTimeScale() == 0.0f)
	{
		return;
	}
	for (int i = 0; i < animations.size(); ++i)
	{
		if (animations[i].state == state)
		{
			if (animQueue.empty())
			{
				if (!interpolating) lastAnim = currAnim;

				currAnim = &animations[i];
				lastCurrentTime = currentTime;

				if (lastAnim != currAnim)
				{
					currentTime = 0.0f;
					loopTime = 0.0f;
					interpolating = true;
				}
				playing = true;
			}
			else
				animQueue.push(currAnim);
			break;
		}
	}
}

void AnimationComponent::GetAnimations()
{
	ImVec2 winPos = ImGui::GetWindowPos();
	ImVec2 size = ImGui::GetWindowSize();
	ImVec2 mouse = ImGui::GetIO().MousePos;
	if (!(mouse.x < winPos.x + size.x && mouse.x > winPos.x &&
		mouse.y < winPos.y + size.y && mouse.y > winPos.y))
	{
		if (ImGui::GetIO().MouseClicked[0]) showAnimMenu = false;
	}

	std::vector<std::string> files;
	app->fs->DiscoverFiles("Library/Animations", files);
	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
	{
		if ((*it).find(".rganim") != std::string::npos)
		{
			app->fs->GetFilenameWithoutExtension(*it);
			*it = (*it).substr((*it).find_last_of("_") + 1, (*it).length());
			uint uid = std::stoll(*it);
			std::shared_ptr<Resource> res = ResourceManager::GetInstance()->GetResource(uid);
			if (ImGui::Selectable(res->GetName().c_str()))
			{
				res->Load();
				//if (currAnim.use_count() - 1 == 1) currAnim->UnLoad();
				SetAnimation(res);
			}
		}
	}
}

const float4x4& AnimationComponent::UpdateBone(Bone* bone, Bone* lastBone, float4x4& parentTransform)
{
	RG_PROFILING_FUNCTION("Updating Bone");

	if (!interpolating)
	{
		bone->Update(currentTime);
	}
	else if (lastBone && lastAnim != currAnim)
	{
		bone->UpdateInterpolation(*lastBone, currentTime, lastCurrentTime, interpolating, interpolatingVel);
		if (!interpolating)
		{
			loopTime = 0.0f;
			currentTime = 0.0f;
		}
	}

	return parentTransform * bone->GetTransform();
}

std::map<std::string, BoneInfo>::const_iterator AnimationComponent::GetBoneInfo(std::string name, const std::map<std::string, BoneInfo>& map)
{
	RG_PROFILING_FUNCTION("Finding Bone Info in Map");
	return map.find(name);
}