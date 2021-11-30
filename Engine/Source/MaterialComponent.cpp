#include "MaterialComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Texture.h"

#include "Imgui/imgui.h"

#include "Profiling.h"

MaterialComponent::MaterialComponent(GameObject* own) : diff(nullptr)
{
	type = ComponentType::MATERIAL;
	diffuse = nullptr;
	checkerImage = nullptr;
	owner = own;
	checker = false;

	// TODO: Rewrite the function on the texture importer to support textures from inside the engine.
	//checkerImage = ResourceManager::GetInstance()->IsTextureLoaded(std::string("Library/Textures/Checker.dds"));
	
	active = true;
}

MaterialComponent::~MaterialComponent()
{
}

void MaterialComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Material"))
	{
		Checkbox(this, "Active", active);
		if (checker)
		{
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", checkerImage->GetAssetsPath().c_str());
			ImGui::Text("Width: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", checkerImage->GetWidth());
			ImGui::Text("Height: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", checkerImage->GetHeight());
			ImGui::Checkbox("Checker Image", &checker);
			ImGui::Image((ImTextureID)checkerImage->GetId(), ImVec2(128, 128));
		}
		else if (diff != nullptr)
		{
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
			ImGui::Checkbox("Checker Image", &checker);
			ImGui::Image((ImTextureID)diff->GetId(), ImVec2(128, 128));
		}
		else
		{
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

	ImGui::PopID();
}

void MaterialComponent::SetNewMaterial(int i, int w, int h, std::string& p)
{
	/*RELEASE(texBuffer);

	texBuffer = new TextureBuffer(i, w, h);
	path = p;

	texBuffer->Unbind();*/
}

bool MaterialComponent::OnLoad(JsonParsing& node)
{
	diffuse = ResourceManager::GetInstance()->IsTextureLoaded(std::string(node.GetJsonString("Path")));
	active = node.GetJsonBool("Active");

	return true;
}

bool MaterialComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Path", diffuse->GetLibraryPath().c_str());
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);
	
	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void MaterialComponent::BindTexture()
{
	if (checker)
	{
		if (checkerImage) checkerImage->Bind();
	}
	else
	{
		if (diff) diff->Bind();
	}
}

void MaterialComponent::UnbindTexture()
{
	if (checker)
	{
		if (checkerImage) checkerImage->Unbind();
	}
	else
	{
		if (diff) diff->Unbind();
	}
}

void MaterialComponent::SetTexture(std::shared_ptr<Resource> tex)
{
	diff = std::static_pointer_cast<Texture>(tex);
}