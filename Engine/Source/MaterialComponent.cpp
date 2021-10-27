#include "MaterialComponent.h"
#include "Globals.h"
#include "Imgui/imgui.h"

#include "mmgr/mmgr.h"

TextureBuffer* MaterialComponent::checkerBuffer = nullptr;

MaterialComponent::MaterialComponent(GameObject* own) : id(0), width(0), height(0), path(""), texBuffer(nullptr)
{
	owner = own;
	checker = false;
	CreateChecker();

	checkerBuffer = new TextureBuffer(0, 64, 64, *checkerImage[0]);
}

MaterialComponent::MaterialComponent(int i, int w, int h, std::string& p) : id(i), width(w), height(h), path(p)
{
	texBuffer = new TextureBuffer(i, w, h);
	checker = false;
	CreateChecker();

	checkerBuffer = new TextureBuffer(0, 64, 64, *checkerImage[0]);

	texBuffer->Unbind();
}

MaterialComponent::MaterialComponent(int i, int w, int h, GLubyte* data) : id(i), width(w), height(h)
{
	texBuffer = new TextureBuffer(i, w, h, data);
	checker = false;
	CreateChecker();

	checkerBuffer = new TextureBuffer(0, 64, 64, *checkerImage[0]);

	texBuffer->Unbind();
}

MaterialComponent::~MaterialComponent()
{
	RELEASE(texBuffer);
}

void MaterialComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Material"))
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