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

ModuleEditor::ModuleEditor() : selected(nullptr), selectedParent(nullptr), Module()
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
					if (ImGui::IsItemHovered() && !gameObjectOptions)
					{
						if (ImGui::GetIO().MouseReleased[1])
						{
							gameObjectOptions = true;
							selected = &obj;
							selectedParent = &object;
						}
						else if (ImGui::GetIO().MouseReleased[0])
						{
							selected = &obj;
							selectedParent = &object;
						}
					}
				}
				ImGui::TreePop();
			}
		}	
	}

	if (gameObjectOptions)
	{
		ImGui::OpenPopup("GameObject");

		if (ImGui::BeginPopup("GameObject"))
		{
			if (ImGui::Button("Move Up", ImVec2(100.0f, 30.0f)))
			{
				selectedParent->MoveChildrenUp(selected);
				gameObjectOptions = false;
			}
			else if (ImGui::Button("Move Down", ImVec2(100.0f, 30.0f)))
			{
				selectedParent->MoveChildrenDown(selected);
				gameObjectOptions = false;
			}
			else if (!ImGui::IsAnyItemHovered() && ((ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1])))
			{
				gameObjectOptions = false;
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();

	

	ImGui::Begin("Inspector");
	if (!app->scene->GetGameObjectsList().empty())
	{
		//GameObject& object = (*app->scene->GetGameObjectsList()[0]);

		// TODO: Uncomment if you want
		/*if (ImGui::BeginMenu(object.GetName()))
		{*/
		if (selected)
		{
			ImGui::SetNextTreeNodeOpen(true);
			// This line below draws in the inspector only the selected object, Unity style
			selected->DrawEditor();


			// TODO: The line below is not necessary anymore, it's "HardCoded", does not function like unity's inspector
			//object.DrawEditor();
		}

		//	ImGui::EndMenu();
		//}
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