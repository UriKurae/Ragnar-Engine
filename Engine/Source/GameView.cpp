#include "GameView.h"
#include "Application.h"

#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"

#include "ModuleSceneManager.h"
#include "Scene.h"

#include "Framebuffer.h"
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
	int flags = ImGuiWindowFlags_NoScrollbar;

	// DELIVERY !!
#if 0
	 = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoScrollbar;
#endif

	if (ImGui::Begin(ICON_FA_GAMEPAD" Game", &active, flags))
	{
		if (ImGui::IsItemActivated() || ImGui::IsItemActive())
			isFocused = true;
		else if (ImGui::IsItemDeactivated() || !ImGui::IsItemActive())
			isFocused = false;

		app->camera->updateGameView = true;

		ImVec2 size = ImGui::GetContentRegionAvail();

		if (sizeViewport.x != size.x || sizeViewport.y != size.y)
		{
			sizeViewport.x = size.x;
			sizeViewport.y = size.y;
			framebuffer->ResizeFramebuffer(size.x, size.y);
			app->renderer3D->OnResize(size.x, size.y);
			app->sceneManager->GetCurrentScene()->mainCamera->UpdateFovAndScreen(size.x, size.y);
		}

		bounds = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, size.x, size.y };
		selected = ImGui::IsWindowFocused();

		ImGui::Image((ImTextureID)framebuffer->GetNormalId(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));
	}
	else
	{
		app->camera->updateGameView = false;
	}

	ImGui::End();
	style.WindowPadding = ImVec2(8.0f, 8.0f);
}