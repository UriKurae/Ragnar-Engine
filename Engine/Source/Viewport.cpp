#include "Viewport.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "Imgui/imgui.h"

Viewport::Viewport()
{
	width = 1080;
	height = 720;
}

Viewport::~Viewport()
{
}

void Viewport::Draw(const unsigned int& framebuffer)
{
	ImGui::Begin("Scene");
	ImGui::Image((ImTextureID)framebuffer, ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}