#include "MaterialComponent.h"
#include "Globals.h"
#include "GameObject.h"
#include "Imgui/imgui.h"

#include "MeshComponent.h"

#include "Profiling.h"

TextureBuffer* MaterialComponent::checkerBuffer = nullptr;

MaterialComponent::MaterialComponent(GameObject* own) : id(0), width(0), height(0), path(""), texBuffer(nullptr)
{
	type = ComponentType::MATERIAL;
	owner = own;
	owner->GetComponent<MeshComponent>()->SetMaterial(this);
	checker = false;
	CreateChecker();

	checkerBuffer = new TextureBuffer(0, 64, 64, *checkerImage[0]);
	active = true;
}

MaterialComponent::MaterialComponent(int i, int w, int h, std::string& p) : id(i), width(w), height(h), path(p)
{
	type = ComponentType::MATERIAL;
	texBuffer = new TextureBuffer(i, w, h);
	checker = false;
	CreateChecker();

	checkerBuffer = new TextureBuffer(0, 64, 64, *checkerImage[0]);

	texBuffer->Unbind();
	active = true;
}

MaterialComponent::MaterialComponent(int i, int w, int h, GLubyte* data) : id(i), width(w), height(h)
{
	type = ComponentType::MATERIAL;
	texBuffer = new TextureBuffer(i, w, h, data);
	checker = false;
	CreateChecker();

	checkerBuffer = new TextureBuffer(0, 64, 64, *checkerImage[0]);

	texBuffer->Unbind();
	active = true;
}

MaterialComponent::~MaterialComponent()
{
	RELEASE(texBuffer);
}

void MaterialComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		Checkbox(this, "Active", active);
		if (checker)
		{
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Default checker's texture");
			ImGui::Text("Width: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", 64);
			ImGui::Text("Height: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", 64);
			ImGui::Checkbox("Checker Image", &checker);
			ImGui::Image((ImTextureID)checkerBuffer->GetID(), ImVec2(128, 128));
		}
		else if (texBuffer != nullptr)
		{
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", path.c_str());
			ImGui::Text("Width: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", width);
			ImGui::Text("Height: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", height);
			ImGui::Checkbox("Checker Image", &checker);
			ImGui::Image((ImTextureID)texBuffer->GetID(), ImVec2(128, 128));
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
}

void MaterialComponent::SetNewMaterial(int i, int w, int h, std::string& p)
{
	RELEASE(texBuffer);

	texBuffer = new TextureBuffer(i, w, h);
	path = p;

	texBuffer->Unbind();
}

bool MaterialComponent::OnLoad(JsonParsing& node, JSON_Array* array)
{
	return true;
}

bool MaterialComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Path", path.c_str());

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void MaterialComponent::BindTexture()
{
	if (checker)
	{
		if (checkerBuffer) checkerBuffer->Bind();
	}
	else
	{
		if (texBuffer) texBuffer->Bind();
	}
}

void MaterialComponent::UnbindTexture()
{
	if (checker)
	{
		if (checkerBuffer) checkerBuffer->Unbind();
	}
	else
	{
		if (texBuffer) texBuffer->Unbind();
	}
}

void MaterialComponent::CreateChecker()
{
	for (int i = 0; i < 64; ++i)
	{
		for (int j = 0; j < 64; ++j)
		{
			GLubyte c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}
}