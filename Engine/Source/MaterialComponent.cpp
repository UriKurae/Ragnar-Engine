#include "MaterialComponent.h"

#include "Application.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Texture.h"

#include "FileSystem.h"

#include "Imgui/imgui.h"

#include "Profiling.h"

MaterialComponent::MaterialComponent(GameObject* own) : diff(nullptr), showTexMenu(false)
{
	type = ComponentType::MATERIAL;
	owner = own;
	checker = false;
	
	active = true;
}

MaterialComponent::MaterialComponent(MaterialComponent* mat) : showTexMenu(false)
{
	checker = mat->checker;
	diff = mat->diff;
}

MaterialComponent::~MaterialComponent()
{
	if (diff.use_count() - 1 == 1) diff->UnLoad();
}

void MaterialComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Material"))
	{
		Checkbox(this, "Active", active);
		if (diff != nullptr)
		{
			if (ImGui::Button(diff ? diff->GetName().c_str() : ""))
			{
				showTexMenu = true;
			}
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", diff->GetAssetsPath().c_str());
			ImGui::Text("Width: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", diff->GetWidth());
			ImGui::Text("Height: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", diff->GetHeight());
			ImGui::Text("Reference Count: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", diff.use_count());
			ImGui::Image((ImTextureID)diff->GetId(), ImVec2(128, 128));
		}
		else
		{
			if (ImGui::Button(""))
			{
				showTexMenu = true;
			}
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "There's no texture");
			ImGui::Text("Width: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", 0);
			ImGui::Text("Height: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", 0);
			ImGui::Checkbox("Checker Image", &checker);
		}
		ImGui::Separator();
	}

	if (showTexMenu)
	{
		ImGui::Begin("Textures", &showTexMenu);
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 size = ImGui::GetWindowSize();
		ImVec2 mouse = ImGui::GetIO().MousePos;
		if (!(mouse.x < winPos.x + size.x && mouse.x > winPos.x &&
			mouse.y < winPos.y + size.y && mouse.y > winPos.y))
		{
			if (ImGui::GetIO().MouseClicked[0]) showTexMenu = false;
		}

		std::vector<std::string> files;
		app->fs->DiscoverFiles("Library/Textures/", files);
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			if ((*it).find(".rgtexture") != std::string::npos)
			{
				app->fs->GetFilenameWithoutExtension(*it);
				*it = (*it).substr((*it).find_last_of("_") + 1, (*it).length());
				uint uid = std::stoll(*it);
				std::shared_ptr<Resource> res = ResourceManager::GetInstance()->LoadResource(uid);
				if (ImGui::Button(res->GetName().c_str(), { ImGui::GetWindowWidth() - 30, 20 }))
				{
					if (diff.use_count() - 1 == 1) diff->UnLoad();
					SetTexture(res);
				}
			}
		}

		ImGui::End();
	}

	ImGui::PopID();
}

bool MaterialComponent::OnLoad(JsonParsing& node)
{
	diff = std::static_pointer_cast<Texture>(ResourceManager::GetInstance()->LoadResource(std::string(node.GetJsonString("Path"))));
	active = node.GetJsonBool("Active");

	return true;
}

bool MaterialComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Path", diff->GetAssetsPath().c_str());
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);
	
	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void MaterialComponent::BindTexture()
{
	if (diff) diff->Bind();
}

void MaterialComponent::UnbindTexture()
{
	if (diff) diff->Unbind();
}

void MaterialComponent::SetTexture(std::shared_ptr<Resource> tex)
{
	diff = std::static_pointer_cast<Texture>(tex);
}