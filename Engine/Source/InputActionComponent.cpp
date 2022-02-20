
#include "InputActionComponent.h"

InputActionComponent::InputActionComponent(GameObject* own, TransformComponent* trans)
{
	owner = own;
	type = ComponentType::INPUT_ACTION;
}

InputActionComponent::~InputActionComponent()
{
}
