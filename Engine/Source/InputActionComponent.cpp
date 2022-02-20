
#include "InputActionComponent.h"
#include "IconsFontAwesome5.h"

InputActionComponent::InputActionComponent(GameObject* own, TransformComponent* trans)
{
	owner = own;
	type = ComponentType::INPUT_ACTION;
}

InputActionComponent::~InputActionComponent()
{
}

void InputActionComponent::OnEditor()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader(ICON_FA_GAMEPAD" Player Input"))
	{

	}
	ImGui::PopID();
}

bool InputActionComponent::Update(float dt)
{
	return false;
}

bool InputActionComponent::OnLoad(JsonParsing& node)
{
	return false;
}

bool InputActionComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	return false;
}
