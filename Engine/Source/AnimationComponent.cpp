#include "AnimationComponent.h"
#include "Application.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "Animation.h"
#include "GameObject.h"

AnimationComponent::AnimationComponent(GameObject* own) : anim(nullptr), showAnimMenu(false)
{
	type = ComponentType::ANIMATION;
	owner = own;

	attackBlendTime = 0.1f;
	walkBlendTime = 0.1f;

	smoothT = true;
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
	if (ImGui::CollapsingHeader("Animation Component"))
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
			if(ImGui::Checkbox("Draw Bones", &debugDraw))
			{
				for (int i = 0; i < owner->GetChilds().size(); i++)
				{
					owner->GetChilds().at(i)->GetComponent<BoneComponent>()->SetDebugDraw();
				}
			}
			
			if (ImGui::CollapsingHeader("Pressing 1, blend animation1"))
			{
				ImGui::Text("Select animation 1: ");
				ImGui::SameLine();
				if (ImGui::Button(anim1 ? anim1->GetName().c_str() : "No Animation"))
				{
					showAnimMenu = true;
				}
			}
			if (ImGui::CollapsingHeader("Pressing 2, blend animation2"))
			{
				ImGui::Text("Select animation 2: ");
				ImGui::SameLine();
				if (ImGui::Button(anim2 ? anim2->GetName().c_str() : "No Animation"))
				{
					showAnimMenu = true;
				}
			}		
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

		ImGui::End();
	}
	ImGui::PopID();
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


