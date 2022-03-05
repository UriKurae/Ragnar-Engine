#include "AnimationComponent.h"
#include "Application.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "Animation.h"
#include "GameObject.h"
#include "IconsFontAwesome5.h"

AnimationComponent::AnimationComponent(GameObject* own) : showAnimMenu(false), deltaTime(0.0f), currAnim(nullptr), playing(false)
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
		//Checkbox(this, "Active", active);
		//if (currAnim != nullptr)
		//{
		//	ImGui::Text("Select animation: ");
		//	ImGui::SameLine();
			//if (ImGui::Button(currAnim ? currAnim->GetName().c_str() : ""))
			//{
			//	showAnimMenu = true;
			//}

		if (ImGui::TreeNodeEx("Animations"))
		{
			for (int i = 0; i < animations.size(); ++i)
			{
				AnimState* currState = &animations[i];
				ImGui::PushID((void*)currState->state.c_str());
				ImGui::InputText("##State", &currState->state[0], 30);
				ImGui::SameLine();
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
						}
					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("##Loop", &currState->loop);
				ImGui::PopID();
			}

			if (ImGui::Button(ICON_FA_PLUS))
			{
				animations.push_back({ "None", nullptr, false });
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_MINUS))
			{
				animations.erase(animations.end() - 1);
			}

			ImGui::TreePop();
		}

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
		/*if(ImGui::Checkbox("Draw Bones", &debugDraw))
		{
			for (int i = 0; i < owner->GetChilds().size(); i++)
			{
				owner->GetChilds().at(i)->GetComponent<BoneComponent>()->SetDebugDraw();
			}
		}*/
		//}
		//else
		//{
		//	ImGui::Text("Select animation: ");
		//	ImGui::SameLine();
		//	if (ImGui::Button("No Animation"))
		//	{
		//		showAnimMenu = true;
		//	}
		//}
		//ImGui::Separator();
	}

	//if (showAnimMenu)
	//{
	//	ImGui::Begin("Animations", &showAnimMenu);
	//	GetAnimations();
	//	ImGui::End();
	//}
	ImGui::PopID();
}

bool AnimationComponent::Update(float dt)
{
	deltaTime = dt;
	if (currAnim && playing)
	{
		if (currentTime > currAnim->anim->GetTicks())
		{
			if (currAnim->loop) currentTime = 0.0f;
			else playing = false;
		}
		currentTime += currAnim->anim->GetTicksPerSecond() * dt;
		currentTime = fmod(currentTime, currAnim->anim->GetTicks());
		CalculateBoneTransform(currAnim->anim->GetHierarchyData(), float4x4::identity);
	}

	return true;
}

void AnimationComponent::CalculateBoneTransform(HierarchyData& data, float4x4 parentTransform)
{
	std::string nodeName = data.name;
	float4x4 nodeTransform = data.transform;

	Bone* bone = currAnim->anim->FindBone(nodeName);

	if (bone)
	{
		bone->Update(currentTime);
		nodeTransform = bone->GetTransform();
	}

	float4x4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = owner->GetComponent<MeshComponent>()->GetBoneMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		float4x4 offset = boneInfoMap[nodeName].offset;
		finalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < data.childrenCount; i++)
		CalculateBoneTransform(data.children[i], globalTransformation);
}


bool AnimationComponent::OnLoad(JsonParsing& node)
{
	//currAnim = std::static_pointer_cast<Animation>(ResourceManager::GetInstance()->LoadResource(std::string(node.GetJsonString("Path"))));
	active = node.GetJsonBool("Active");
	return true;
}

bool AnimationComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	//file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Path", currAnim->GetAssetsPath().c_str());
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void AnimationComponent::SetAnimation(std::shared_ptr<Resource> a)
{
	//currAnim = std::static_pointer_cast<Animation>(a);
}

void AnimationComponent::Play(std::string state)
{
	for (int i = 0; i < animations.size(); ++i)
	{
		if (animations[i].state == state)
		{
			currAnim = &animations[i];
			playing = true;
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