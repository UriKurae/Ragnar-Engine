#include "AnimationComponent.h"
#include "Application.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "Animation.h"
#include "GameObject.h"
#include "IconsFontAwesome5.h"

AnimationComponent::AnimationComponent(GameObject* own) : anim(nullptr), showAnimMenu(false)
{
	type = ComponentType::ANIMATION;
	owner = own;

	currentTime = 0.0f;

	finalBoneMatrices.reserve(100);

	for (int i = 0; i < 100; i++)
	{
		finalBoneMatrices.push_back(float4x4::identity);
	}

	active = true;
}

AnimationComponent::AnimationComponent(AnimationComponent* animation) : showAnimMenu(false)
{
	anim = animation->anim;
}

AnimationComponent::~AnimationComponent()
{
	if (anim.use_count() - 1 == 1) anim->UnLoad();
}

void AnimationComponent::OnEditor()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader(ICON_FA_PEOPLE_ARROWS" Animation Component"))
	{
		Checkbox(this, "Active", active);
		if (anim != nullptr)
		{
			ImGui::Text("Select animation: ");
			ImGui::SameLine();
			if (ImGui::Button(anim ? anim->GetName().c_str() : ""))
			{
				showAnimMenu = true;
			}
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", anim->GetAssetsPath().c_str());
			ImGui::Text("Ticks: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.0f", anim->GetTicks());
			ImGui::Text("Ticks Per Second: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.0f", anim->GetTicksPerSecond());
			ImGui::Text("Duration: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.0f s", anim->GetDuration());
			ImGui::Text("Bones Attached: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d bones", anim->numBones);
			/*if(ImGui::Checkbox("Draw Bones", &debugDraw))
			{
				for (int i = 0; i < owner->GetChilds().size(); i++)
				{
					owner->GetChilds().at(i)->GetComponent<BoneComponent>()->SetDebugDraw();
				}
			}*/
		}
		else
		{
			ImGui::Text("Select animation: ");
			ImGui::SameLine();
			if (ImGui::Button("No Animation"))
			{
				showAnimMenu = true;
			}
		}
		ImGui::Separator();
	}

	if (showAnimMenu)
	{
		ImGui::Begin("Animations", &showAnimMenu);
		GetAnimations();
		ImGui::End();
	}
	ImGui::PopID();
}

bool AnimationComponent::Update(float dt)
{
	deltaTime = dt;
	if (anim)
	{
		currentTime += anim->GetTicksPerSecond() * dt;
		currentTime = fmod(currentTime, anim->GetDuration());
		CalculateBoneTransform(/*anim->GetRootNode(),*/ float4x4::identity);
	}
}

void AnimationComponent::CalculateBoneTransform(/*const AssimpNodeData* node,*/ float4x4 parentTransform)
{
	//std::string nodeName = node->name;
	//float4x4 nodeTransform = node->transformation;

	//Bone* bone = anim->FindBone(nodeName);

	//if (bone)
	//{
	//	bone->Update(currentTime);
	//	nodeTransform = bone->GetLocalTransform();
	//}

	//float4x4 globalTransformation = parentTransform * nodeTransform;

	//auto boneInfoMap = anim->GetBoneIDMap();
	//if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	//{
	//	int index = boneInfoMap[nodeName].id;
	//	float4x4 offset = boneInfoMap[nodeName].offset;
	//	finalBoneMatrices[index] = globalTransformation * offset;
	//}

	//for (int i = 0; i < node->childrenCount; i++)
	//	CalculateBoneTransform(&node->children[i], globalTransformation);
}


bool AnimationComponent::OnLoad(JsonParsing& node)
{
	anim = std::static_pointer_cast<Animation>(ResourceManager::GetInstance()->LoadResource(std::string(node.GetJsonString("Path"))));
	active = node.GetJsonBool("Active");
	return true;
}

bool AnimationComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Path", anim->GetAssetsPath().c_str());
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void AnimationComponent::SetAnimation(std::shared_ptr<Resource> a)
{
	anim = std::static_pointer_cast<Animation>(a);
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
				if (anim.use_count() - 1 == 1) anim->UnLoad();
				SetAnimation(res);
			}
		}
	}
}