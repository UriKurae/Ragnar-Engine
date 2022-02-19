#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "Viewport.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"

#include "CommandsDispatcher.h"
#include "GameObjectCommands.h"

#include "FileSystem.h"
#include "ResourceManager.h"

#include "Imgui/imgui.h"
#include "Imgui/ImGuizmo.h"
#include "Globals.h"

#include "IconsFontAwesome5.h"

#include "Profiling.h"

Viewport::Viewport()
{
	sizeViewport = float2(0,0);
	active = true;
}

Viewport::~Viewport()
{
	CommandDispatcher::Shutdown();
}

void Viewport::Draw(Framebuffer* framebuffer, Framebuffer* gameBuffer, int currentOperation)
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(0.0f, 0.0f);

	if (ImGui::Begin(ICON_FA_EYE" Scene", &active, ImGuiWindowFlags_NoScrollbar))
	{
		app->camera->canBeUpdated = true;	

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

		if (app->editor->GetGO())
		{
			ImGuizmo::Enable(true);
			ImGuizmo::SetGizmoSizeClipSpace(0.3f);
			ImGuizmo::SetRect(bounds.x, bounds.y, size.x, size.y);
			ImGuizmo::SetDrawlist();

			math::float4x4 view = app->camera->cameraFrustum.ViewMatrix();

			math::float4x4 tr = app->editor->GetGO()->GetComponent<TransformComponent>()->GetGlobalTransform().Transposed();
			ImGuizmo::Manipulate(view.Transposed().ptr(), app->camera->cameraFrustum.ProjectionMatrix().Transposed().ptr(), (ImGuizmo::OPERATION)currentOperation, ImGuizmo::MODE::LOCAL, tr.ptr());
			static bool firstMove = false;
			if (ImGuizmo::IsUsing())
			{
				GameObject* go = app->editor->GetGO();
				if (!firstMove)
				{
					firstMove = true;

					CommandDispatcher::Execute(new MoveGameObjectCommand(go));
				}
				go->GetComponent<TransformComponent>()->SetTransform(tr.Transposed());
			}
			else firstMove = false;
		}

		// TODO: Not the best place to call this
		if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KeyState::KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_Z) == KeyState::KEY_UP)
		{
			CommandDispatcher::Undo();
		}
		if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KeyState::KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_Y) == KeyState::KEY_UP)
		{
			CommandDispatcher::Redo();
		}



		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content Browser"))
			{
				// TODO: Check this for dropping resources to the scene
				const char* path = (const char*)payload->Data;
				std::string scene = path;
				if (scene.find(".ragnar") != std::string::npos)
				{
					app->scene->LoadScene(scene.c_str());
				}
				else
				{
					ResourceManager::GetInstance()->LoadResource(std::string(path));
				}
			}
			ImGui::EndDragDropTarget();
		}

		GameObject* camera = app->editor->GetGO();
		if (camera && camera->GetComponent<CameraComponent>())
		{
			ImGui::SetNextWindowSize(ImVec2(210, 150));
			ImGui::SetNextWindowPos(ImVec2((bounds.x + bounds.z) - 225, (bounds.y + bounds.w) - 150));

			bool ret = true;
			ImGui::Begin("Game Preview", &ret, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

			ImGui::Image((ImTextureID)gameBuffer->GetId(), ImVec2(210, 150), ImVec2(0, 1), ImVec2(1, 0));

			ImGui::End();
		}
	}
	else
	{
		app->camera->canBeUpdated = false;
	}

	ImGui::End();
	style.WindowPadding = ImVec2(8.0f, 8.0f);
}