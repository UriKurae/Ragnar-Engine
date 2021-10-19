#include "TransformComponent.h"

#include "Imgui/imgui.h"

TransformComponent::TransformComponent()
{
	position = { 0.0f, 0.0f, 0.0f }; 
	rotation = { 0.0f, 1.0f, 1.0f, 1.0f };
	scale = { 1.0f, 1.0f, 1.0f };
}

TransformComponent::~TransformComponent()
{
}

bool TransformComponent::Update(float dt)
{
	return true;
}

void TransformComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		std::string test = std::to_string(position.x);
		char* pos = new char[test.length()];
		strcpy(pos, test.c_str());
		ImGui::InputText("Position", pos, 20);
		position.x = atof(pos);
		//ImGui::InputFloat("Position", &position.x, 1);
		/*float *test[3] = { &position.x, &position.y, &position.z };
		ImGui::DragFloat3("Pos", *test);*/

	}
}

void TransformComponent::SetTransform(float3 pos, Quat rot, float3 sca)
{
	position = pos;
	rotation = rot;
	scale = sca;
}