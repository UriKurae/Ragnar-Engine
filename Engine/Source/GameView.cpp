#include "Application.h"

#include "CameraComponent.h"
#include "GameView.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"

#include "Imgui/imgui.h"

#include "Profiling.h"

GameView::GameView()
{
	sizeViewport = float2(0, 0);
	active = true;
}

GameView::~GameView()
{
}

void GameView::Draw(Framebuffer* framebuffer)
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(0.0f, 0.0f);

	ImGui::Begin("Game", &active, ImGuiWindowFlags_NoScrollbar);

	ImVec2 size = ImGui::GetContentRegionAvail();

	if (sizeViewport.x != size.x || sizeViewport.y != size.y)
	{
		sizeViewport.x = size.x;
		sizeViewport.y = size.y;
		framebuffer->ResizeFramebuffer(size.x, size.y);
		app->renderer3D->OnResize(size.x, size.y);
		app->scene->mainCamera->UpdateFovAndScreen(size.x, size.y);
	}
	bounds = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, size.x, size.y };
	selected = ImGui::IsWindowFocused();

	ImGui::Image((ImTextureID)framebuffer->GetId(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
	style.WindowPadding = ImVec2(8.0f, 8.0f);
}