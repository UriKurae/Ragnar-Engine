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