#include "Application.h"
#include "ModuleCamera3D.h"
#include "Viewport.h"
#include "ModuleRenderer3D.h"
#include "CameraComponent.h"
#include "ModuleScene.h"

#include "FileSystem.h"

#include "Imgui/imgui.h"

Viewport::Viewport()
{
	sizeViewport = float2(0,0);
	active = true;
}

Viewport::~Viewport()
{
}

void Viewport::Draw(Framebuffer* framebuffer)
{

	ImGui::Begin("Scene", &active);
	
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

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content Browser"))
		{
			const char* path = (const char*)payload->Data;
			app->fs->LoadFile(std::string(path));
		}
		ImGui::EndDragDropTarget();
	}


	if (selected)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(app->camera->matrixViewFrustum.Transposed().ptr());
		glPopMatrix();
		ImGui::Image((ImTextureID)framebuffer->GetId(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));
	}

	ImGui::End();

	ImGui::Begin("Game");
	if (!selected)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(app->scene->mainCamera->matrixViewFrustum.Transposed().ptr());
		glPopMatrix();
		ImGui::Image((ImTextureID)framebuffer->GetId(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));
	}
	ImGui::End();
	
	
}