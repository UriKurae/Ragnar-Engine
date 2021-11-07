#include "MaterialComponent.h"
#include "Globals.h"
#include "GameObject.h"
#include "Imgui/imgui.h"
#include "TextureLoader.h"
#include "Texture.h"

#include "MeshComponent.h"

#include "Profiling.h"

MaterialComponent::MaterialComponent(GameObject* own)
{
	type = ComponentType::MATERIAL;
	diffuse = nullptr;
	checkerImage = nullptr;
	owner = own;
	checker = false;

	TextureLoader::GetInstance()->LoadTexture(std::string("Checker"), &checkerImage);
	active = true;
}

MaterialComponent::~MaterialComponent()
{
	RELEASE(diffuse);
	RELEASE(checkerImage);
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
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", checkerImage->GetPath().c_str());
			ImGui::Text("Width: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", checkerImage->GetWidth());
			ImGui::Text("Height: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", checkerImage->GetHeight());
			ImGui::Checkbox("Checker Image", &checker);
			ImGui::Image((ImTextureID)checkerImage->GetId(), ImVec2(128, 128));
		}
		else if (diffuse != nullptr)
		{
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", diffuse->GetPath().c_str());
			ImGui::Text("Width: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", diffuse->GetWidth());
			ImGui::Text("Height: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", diffuse->GetHeight());
			ImGui::Checkbox("Checker Image", &checker);
			ImGui::Image((ImTextureID)diffuse->GetId(), ImVec2(128, 128));
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
	RELEASE(diffuse);
	TextureLoader::GetInstance()->LoadTexture(std::string(node.GetJsonString("Path")), this);
	active = node.GetJsonBool("Active");

	return true;
}

bool MaterialComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Path", diffuse->GetPath().c_str());
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
		if (diffuse) diffuse->Bind();
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
		if (diffuse) diffuse->Unbind();
	}
}