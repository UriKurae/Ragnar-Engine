#include "Application.h"
#include "ModuleCamera3D.h"
#include "CameraComponent.h"
#include "ModuleEditor.h"
#include "GameView.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"

#include "FileSystem.h"

#include "Imgui/imgui.h"

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

	ImGui::Begin("Game", &active);
	if (ImGui::IsItemActive()) app->renderer3D->currentView = CurrentView::GAME;

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
}
