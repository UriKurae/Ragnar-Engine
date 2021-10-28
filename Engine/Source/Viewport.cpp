#include "Viewport.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "Imgui/imgui.h"

Viewport::Viewport()
{
	sizeViewport = float2(0,0);
}

Viewport::~Viewport()
{
}

void Viewport::Draw(Framebuffer* framebuffer)
{
	ImGui::Begin("Scene");

	ImVec2 size = ImGui::GetContentRegionAvail();
	
	if (sizeViewport.x != size.x || sizeViewport.y != size.y)
	{
		sizeViewport.x = size.x;
		sizeViewport.y = size.y;
		framebuffer->ResizeFramebuffer(size.x, size.y);
		app->renderer3D->OnResize(size.x, size.y);

	}
	bounds = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, size.x, size.y };
	selected = ImGui::IsWindowFocused();

	ImGui::Image((ImTextureID)framebuffer->GetId(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}