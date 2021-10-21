#include "TransformComponent.h"
#include "Globals.h"

#include "Imgui/imgui.h"

#include "mmgr/mmgr.h"

TransformComponent::TransformComponent()
{
	position = { 0.0f, 0.0f, 0.0f }; 
	rotation = { 0.0f, 1.0f, 1.0f, 1.0f };
	scale = { 1.0f, 1.0f, 1.0f };
	transform = new GLfloat[16];
}

TransformComponent::~TransformComponent()
{
	RELEASE_ARRAY(transform);
}

bool TransformComponent::Update(float dt)
{
	return true;
}

void TransformComponent::OnEditor()
{
	ImGui::SetNextTreeNodeOpen(true);
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::PushItemWidth(90);
		std::string test = std::to_string(position.x);
		char* pos = new char[test.length()];
		strcpy(pos, test.c_str());
		
		ShowTransformationInfo();

		ImGui::Separator();
	}
}

void TransformComponent::SetTransform(float3 pos, Quat rot, float3 sca)
{
	position = pos;
	rotation = rot;
	scale = sca;

	float4x4 trans = float4x4::FromTRS(position, rotation, scale);

	for (int j = 0; j < 4; ++j)
	{
		for (int i = 0; i < 4; ++i)
		{
			transform[(j * 4) + i] = trans[i][j];
		}
	}

	//transform[0] = trans.scaleX;
	//transform[1] = trans.shearXy;
	//transform[2] = trans.shearXz;
	//transform[3] = trans.x;
	
	//transform[4] = trans.shearYx;
	//transform[5] = trans.scaleY;
	//transform[6] = trans.shearYz;
	//transform[7] = trans.y;

	//transform[8] = trans.shearZx;
	//transform[9] = trans.shearZy;
	//transform[10] = trans.scaleZ;
	//transform[11] = trans.z;

	//transform[12] = trans.shearWx;
	//transform[13] = trans.shearWy;
	//transform[14] = trans.shearWz;
	//transform[15] = trans.w;
}

void TransformComponent::ShowTransformationInfo()
{
	// Show Position
	ImGui::Text("Position");
	ImGui::SameLine();
	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::InputFloat("", &position.x);
	ImGui::SameLine();
	ImGui::Text("Y");
	ImGui::SameLine();
	ImGui::InputFloat("", &position.y);
	ImGui::SameLine();
	ImGui::Text("Z");
	ImGui::SameLine();
	ImGui::InputFloat("", &position.z);

	// Show Rotation
	ImGui::Text("Rotation");
	ImGui::SameLine();
	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::InputFloat("", &rotation.x);
	ImGui::SameLine();
	ImGui::Text("Y");
	ImGui::SameLine();
	ImGui::InputFloat("", &rotation.y);
	ImGui::SameLine();
	ImGui::Text("Z");
	ImGui::SameLine();
	ImGui::InputFloat("", &rotation.z);


	// Show Scale
	ImGui::Text("Scale");
	ImGui::SameLine(72.5f);
	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::InputFloat("", &scale.x);
	ImGui::SameLine();
	ImGui::Text("Y");
	ImGui::SameLine();
	ImGui::InputFloat("", &scale.y);
	ImGui::SameLine();
	ImGui::Text("Z");
	ImGui::SameLine();
	ImGui::InputFloat("", &scale.z);

}
