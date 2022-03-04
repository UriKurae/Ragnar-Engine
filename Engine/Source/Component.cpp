#include "Component.h"
#include "GameObject.h"

void Component::ComponentOptions(Component* component)
{
	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Move Up").x - 50 - ImGui::CalcTextSize("Move Down").x - 50 - ImGui::CalcTextSize("Delete").x);
	if (ImGui::Button(ICON_FA_ARROW_UP" Move Up"))
		owner->MoveComponent(component, +1);
	
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_ARROW_DOWN" Move Down"))
		owner->MoveComponent(component, -1);

	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_TRASH" Delete"))
		owner->RemoveComponent(component);
}