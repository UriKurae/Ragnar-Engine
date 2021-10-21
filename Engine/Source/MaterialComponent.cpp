#include "MaterialComponent.h"
#include "Imgui/imgui.h"

MaterialComponent::MaterialComponent() : height(0), width(0), id(0), path("")
{
}

MaterialComponent::~MaterialComponent()
{
}

void MaterialComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::Text("Path: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", path.c_str());
		ImGui::Separator();
	}
}