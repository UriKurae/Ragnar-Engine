#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "Viewport.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"

#include "FileSystem.h"

#include "Imgui/imgui.h"
#include "Imgui/ImGuizmo.h"

Viewport::Viewport()
{
	sizeViewport = float2(0,0);
	active = true;
}

Viewport::~Viewport()
{
}

void Viewport::Draw(Framebuffer* framebuffer, int currentOperation)
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

	ImGui::Image((ImTextureID)framebuffer->GetId(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();

	ImGui::Begin("Scene");
	if (ImGui::IsItemActive()) app->renderer3D->currentView = CurrentView::EDITOR;
	ImGui::Image((ImTextureID)framebuffer->GetId(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));

	if (app->editor->GetSelected())
	{
		ImGuizmo::Enable(true);
		ImGuizmo::SetGizmoSizeClipSpace(0.3f);
		ImGuizmo::SetRect(bounds.x, bounds.y, bounds.z, bounds.w);
		ImGuizmo::SetDrawlist();
		
		math::float4x4 view = app->camera->cameraFrustum.ViewMatrix();
		view.Transpose();

		math::float4x4 tr = app->editor->GetSelected()->GetComponent<TransformComponent>()->GetTransform().Transposed();
		ImGuizmo::Manipulate(view.ptr(), app->camera->cameraFrustum.ProjectionMatrix().Transposed().ptr(), (ImGuizmo::OPERATION)currentOperation, ImGuizmo::MODE::LOCAL, tr.ptr());
		if (ImGuizmo::IsUsing())
		{
			app->editor->GetSelected()->GetComponent<TransformComponent>()->SetTransform(tr.Transposed());
		}
	}

	ImGui::End();
}