#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "Viewport.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "TransformComponent.h"

#include "CommandsDispatcher.h"
#include "GameObjectCommands.h"

#include "FileSystem.h"
#include "ResourceManager.h"
#include "PrefabManager.h"

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

void Viewport::Draw(Framebuffer* framebuffer, Framebuffer* gameBuffer)
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(0.0f, 0.0f);

	if (ImGui::Begin(ICON_FA_EYE" Scene", &active, ImGuiWindowFlags_NoScrollbar))
	{
		if (ImGui::IsItemActivated() || ImGui::IsItemActive())
			isFocused = true;
		else if (ImGui::IsItemDeactivated()|| !ImGui::IsItemActive())
			isFocused = false;

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

		GameObject* goSel = app->editor->GetGO();
		if (goSel)
		{
			ImGuizmo::Enable(true);
			ImGuizmo::SetGizmoSizeClipSpace(0.3f);
			ImGuizmo::SetRect(bounds.x, bounds.y, size.x, size.y);
			ImGuizmo::SetDrawlist();

			math::float4x4 view = app->camera->cameraFrustum.ViewMatrix();
			math::float4x4 tr = goSel->GetComponent<TransformComponent>()->GetGlobalTransform().Transposed();

			ImGuizmo::Manipulate(view.Transposed().ptr(), app->camera->cameraFrustum.ProjectionMatrix().Transposed().ptr(), currentOperation, ImGuizmo::MODE::LOCAL, tr.ptr(), 0, (float*)snap);
			static bool firstMove = false;
			if (ImGuizmo::IsUsing())
			{
				GameObject* go = goSel;
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
				else if (scene.find(".rgprefab") != std::string::npos)
				{
					PrefabManager::GetInstance()->LoadPrefab(scene.c_str());
				}
				else
				{
					ResourceManager::GetInstance()->LoadResource(std::string(path));
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (goSel && goSel->GetComponent<CameraComponent>())
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

void Viewport::SetSnap(ImGuizmo::OPERATION operation)
{
	bool desactiveSnap = false;
	switch (operation)
	{
	case ImGuizmo::OPERATION::TRANSLATE:
		currentOperation = ImGuizmo::OPERATION::TRANSLATE;
		if(translateSnap) snap[0] = snap[1] = snap[2] = allTsnap;
		else snap[0] = snap[1] = snap[2] = 0;
		break;
	case ImGuizmo::OPERATION::ROTATE:
		currentOperation = ImGuizmo::OPERATION::ROTATE;
		if (rotateSnap) snap[0] = snap[1] = snap[2] = allRsnap;
		else snap[0] = snap[1] = snap[2] = 0;
		break;
	case ImGuizmo::OPERATION::SCALE:
		currentOperation = ImGuizmo::OPERATION::SCALE;
		if (scaleSnap) snap[0] = snap[1] = snap[2] = allSsnap;
		else snap[0] = snap[1] = snap[2] = 0;
		break;
	default:
		break;
	}
}

void Viewport::SnapOptions()
{
	ImGui::Checkbox("Snap Translate", &translateSnap);
	ImGui::SliderFloat("##Translate", &allTsnap, 0.0f, 10.0f, "%.2f");

	ImGui::Checkbox("Snap Rotation", &rotateSnap);
	ImGui::SliderInt("##Rotation", &allRsnap, 0.0f, 90.0f);

	ImGui::Checkbox("Snap Scale", &scaleSnap);
	ImGui::SliderFloat("##Scale", &allSsnap, 0.0f, 5.0f, "%.2f");

	SetSnap(currentOperation);
}


