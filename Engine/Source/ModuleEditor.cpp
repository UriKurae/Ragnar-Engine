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

#include <GL/glew.h>

#include "Optick/include/optick.h"
#include "mmgr/mmgr.h"

ModuleEditor::ModuleEditor() : Module()
{
	name = "Editor";
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Update(float dt)
{
	OPTICK_EVENT("Updating Module Editor");
	bool ret = true;

	ret = mainMenuBar.Update(dt);

	if (app->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		app->window->SetFullscreen();
	}

	ImGui::Begin("Hierarchy");
	for (int i = 0; i < app->scene->GetGameObjectsList().size(); ++i)
	{
		GameObject& object = (*app->scene->GetGameObjectsList()[i]);
		if (object.GetParent() == nullptr)
		{
			if (ImGui::TreeNode(object.GetName()))
			{
				for (int i = 0; i < object.GetChilds().size(); ++i)
				{
					GameObject& obj = (*object.GetChilds()[i]);
					if (ImGui::TreeNode(obj.GetName()))
					{
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();

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

bool ModuleEditor::Draw()
{
	OPTICK_EVENT("Drawing Module Editor");
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return true;
}

bool ModuleEditor::CleanUp()
{
	mainMenuBar.CleanUp();

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