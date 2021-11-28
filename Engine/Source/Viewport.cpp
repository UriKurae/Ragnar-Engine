#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "Viewport.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"

#include "FileSystem.h"

#include "Imgui/imgui.h"
#include "Imgui/ImGuizmo.h"

#include "Profiling.h"

Viewport::Viewport()
{
	sizeViewport = float2(0,0);
	active = true;
}

Viewport::~Viewport()
{
}

void Viewport::Draw(Framebuffer* framebuffer, Framebuffer* gameBuffer, int currentOperation)
{
	ImGui::Begin("Scene");
	if (ImGui::IsItemActive()) app->renderer3D->currentView = CurrentView::EDITOR;
	

	ImVec2 size = ImGui::GetContentRegionAvail();

	if (sizeViewport.x != size.x || sizeViewport.y != size.y)
	{
		sizeViewport.x = size.x;
		sizeViewport.y = size.y;
		framebuffer->ResizeFramebuffer(size.x, size.y);
		app->renderer3D->OnResize(size.x, size.y);
		app->camera->UpdateFovAndScreen(size.x, size.y);
	}

	bounds = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, size.x, size.y };
	selected = ImGui::IsWindowFocused();

	ImGui::Image((ImTextureID)framebuffer->GetId(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));

	if (app->editor->GetSelected())
	{
		ImGuizmo::Enable(true);
		ImGuizmo::SetGizmoSizeClipSpace(0.3f);
		ImGuizmo::SetRect(bounds.x, bounds.y, size.x, size.y);
		ImGuizmo::SetDrawlist();
		
		math::float4x4 view = app->camera->cameraFrustum.ViewMatrix();

		math::float4x4 tr = app->editor->GetSelected()->GetComponent<TransformComponent>()->GetLocalTransform().Transposed();
		ImGuizmo::Manipulate(view.Transposed().ptr(), app->camera->cameraFrustum.ProjectionMatrix().Transposed().ptr(), (ImGuizmo::OPERATION)currentOperation, ImGuizmo::MODE::LOCAL, tr.ptr());
		if (ImGuizmo::IsUsing())
		{
			app->editor->GetSelected()->GetComponent<TransformComponent>()->SetTransform(tr.Transposed());
		}
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content Browser"))
		{
			const char* path = (const char*)payload->Data;
			app->fs->LoadFile(std::string(path));
		}
		ImGui::EndDragDropTarget();
	}
	

	GameObject* camera = app->editor->GetSelected();
	if (camera && camera->GetComponent<CameraComponent>())
	{
		ImGui::SetNextWindowSize(ImVec2(225, 150));
		ImGui::SetNextWindowPos(ImVec2((bounds.x + bounds.z) - 225, (bounds.y + bounds.w) - 150));

		ImGui::Begin("Game Preview");

		ImGui::Image((ImTextureID)gameBuffer->GetId(), ImVec2(200, 120), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
	}
	

	ImGui::End();
}