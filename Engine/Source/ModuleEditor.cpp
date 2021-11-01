#include "ModuleEditor.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "GameObject.h"

#include "ConsoleMenu.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "Imgui/imgui_impl_sdl.h"

#include <GL/glew.h>

#include "Profiling.h"

ModuleEditor::ModuleEditor() : selected(nullptr), selectedParent(nullptr), createGameObject(false), gameObjectOptions(false), Module()
{
	name = "Editor";

	viewport = new Viewport();
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Update(float dt)
{
	RG_PROFILING_FUNCTION("Updating Module Editor");
	bool ret = true;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport();
	
	ret = mainMenuBar.Update(dt);

	if (app->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		app->window->SetFullscreen();
	}


	/*ImGui::Begin("Hierarchy");
	if (ImGui::Button("+"))
	{
		createGameObject = true;
	}
	int size = app->scene->GetGameObjectsList().size();
	GameObject* root = app->scene->GetRoot();
	ImGuiTreeNodeFlags flags = ((selected == root) ? ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow;
	if (ImGui::TreeNodeEx(root, flags, root->GetName()))
	{
		ShowChildren(root);
		ImGui::TreePop();
	}

	if (gameObjectOptions)
	{
		ImGui::OpenPopup("GameObject");

		if (ImGui::BeginPopup("GameObject"))
		{
			if (ImGui::Button("Move Up", ImVec2(100.0f, 30.0f)))
			{
				if (selectedParent != nullptr)
				{
					selectedParent->MoveChildrenUp(selected);
				}
				else
				{
					app->scene->MoveGameObjectUp(selected);
				}
				gameObjectOptions = false;
			}
			else if (ImGui::Button("Move Down", ImVec2(100.0f, 30.0f)))
			{
				if (selectedParent != nullptr)
				{
					selectedParent->MoveChildrenDown(selected);
				}
				else
				{
					app->scene->MoveGameObjectDown(selected);
				}
				gameObjectOptions = false;
			}
			else if (!ImGui::IsAnyItemHovered() && ((ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1])))
			{
				selected = nullptr;
				selectedParent = nullptr;
				gameObjectOptions = false;
			}
			ImGui::EndPopup();
		}
	}
	else if (createGameObject)
	{
		ImGui::OpenPopup("Create GameObject");
		if (ImGui::BeginPopup("Create GameObject"))
		{
			if (ImGui::Button("Create Empty Object"))
			{
				if (selected != nullptr) app->scene->CreateGameObject(selected);
				else app->scene->CreateGameObject(nullptr);
				createGameObject = false;
			}
			else if (ImGui::Button("Create Cube"))
			{
				if (selected != nullptr) app->scene->Create3DObject(Object3D::CUBE, selected);
				else app->scene->Create3DObject(Object3D::CUBE, nullptr);
				createGameObject = false;
			}
			else if (ImGui::Button("Create Pyramide"))
			{
				if (selected != nullptr) app->scene->Create3DObject(Object3D::PYRAMIDE, selected);
				else app->scene->Create3DObject(Object3D::PYRAMIDE, nullptr);
				createGameObject = false;
			}
			else if (ImGui::Button("Create Sphere"))
			{
				if (selected != nullptr) app->scene->Create3DObject(Object3D::SPHERE, selected);
				else app->scene->Create3DObject(Object3D::SPHERE, nullptr);
				createGameObject = false;
			}
			else if (ImGui::Button("Create Cylinder"))
			{
				if (selected != nullptr) app->scene->Create3DObject(Object3D::CYLINDER, selected);
				else app->scene->Create3DObject(Object3D::CYLINDER, nullptr);
				createGameObject = false;
			}
			else if (!ImGui::IsAnyItemHovered() && ((ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1])))
			{
				createGameObject = false;
			}
			ImGui::EndPopup();
		}
	}

	ImGui::End();*/

	//for (int i = 0; i < app->scene->GetGameObjectsList().size(); i++)
	//{
	//	GameObject& object = (*app->scene->GetGameObjectsList()[i]);
	//	if (ImGui::TreeNodeEx(object.GetName()))
	//	{
	//		ImGui::TreePop();
	//	}
	//}
	//ImGui::End();

	return ret;
}

bool ModuleEditor::Draw(Framebuffer* framebuffer)
{
	RG_PROFILING_FUNCTION("Drawing Module Editor");
	
	viewport->Draw(framebuffer);
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return true;
}

bool ModuleEditor::CleanUp()
{
	mainMenuBar.CleanUp();

	RELEASE(viewport);

	return true;
}

bool ModuleEditor::LoadConfig(JsonParsing& node)
{
	return true;
}

bool ModuleEditor::SaveConfig(JsonParsing& node) const
{
	return true;
}

void ModuleEditor::LogConsole(const char* string)
{
	if (mainMenuBar.GetConsole()) mainMenuBar.GetConsole()->AddLog(string);
}

void ModuleEditor::ShowChildren(GameObject* parent)
{
	int size = parent->GetChilds().size();
	for (int i = 0; i < size; ++i)
	{
		GameObject* obj = parent->GetChilds()[i];
		ImGuiTreeNodeFlags flags = ((selected == obj) ? ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = false;
		if (obj != nullptr) opened = ImGui::TreeNodeEx((void*)obj, flags, obj->GetName());
		if (ImGui::IsItemClicked())
		{
			selected = obj;
			selectedParent = parent;
		}
		else if (ImGui::IsItemClicked(1))
		{
			selected = obj;
			selectedParent = parent;
			gameObjectOptions = true;
		}

		if (opened)
		{
			ShowChildren(obj);
			ImGui::TreePop();
		}

		if (!ImGui::IsAnyItemHovered() && (ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1]))
		{
			selected = nullptr;
			selectedParent = nullptr;
		}
	}
}