#include "ModuleEditor.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#include "Imgui/imgui.h"

ModuleEditor::ModuleEditor(Application* a) : Module(a)
{
	showMenu = true;
	showHelpMenu = false;
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Update(float dt)
{
	if (ImGui::BeginMainMenuBar())
	{
		bool ret = false;

		if (ImGui::BeginMenu("File"))
		{
			// Project options (Create, open...)
			ImGui::MenuItem("New Project", NULL, &ret);
			ImGui::MenuItem("Open Project", NULL, &ret);

			ImGui::Separator();

			ImGui::MenuItem("Save", "Ctrl + S", &ret);
			if (ImGui::MenuItem("Exit", "Alt + F4", &ret))
			{
				return false;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("Undo", "Ctrl + Z", &ret);
			ImGui::MenuItem("Redo", "Ctrl + Y", &ret);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("Demo Menu", NULL, &showMenu);
			if (ImGui::MenuItem("Fullscreen", "Alt+Enter", &ret))
			{
				app->window->SetFullscreen();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About Ragnar Engine", NULL, &showHelpMenu);
			if (ImGui::MenuItem("Documentation", "F1", &ret))
			{
				app->RequestBrowser("https://github.com/UriKurae/Ragnar-Engine");
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	if (showMenu)
	{
		ImGui::ShowDemoWindow(&showMenu);

		ImGui::Begin("Ragnar Engine", &showMenu);
		if (ImGui::Button("Close", ImVec2(0, 0)))
		{
			return false;
		}
		ImGui::End();

		//ImGui::Button("File", ImVec2(0, 0));
		//ImGui::Button("Edit", ImVec2(0, 0));

		//if (ImGui::CollapsingHeader("Tab"))
		//{
		//	ImGui::TreeNode("Close");
		//}
	}

	fps.emplace_back(ImGui::GetIO().Framerate);
	ms.emplace_back(1000.0f / ImGui::GetIO().Framerate);

	bool r = true;
	ImGui::Begin("Configuration", &r);
	if (ImGui::CollapsingHeader("Application"))
	{
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps[fps.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps[0], fps.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %0.1f", ms[ms.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &ms[0], ms.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}
	if (ImGui::CollapsingHeader("Window"))
	{
		bool fullscreen = false;
		if (ImGui::Checkbox("Fullscreen", &fullscreen))
		{
			fullscreen = !fullscreen;
			app->window->SetFullscreen();
		}
		bool resizable = false;
		if (ImGui::Checkbox("Resizable", &resizable))
		{
			resizable = !resizable;
			app->window->SetResizable();
		}
	}
	ImGui::End();


	if (app->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		app->window->SetFullscreen();
	}

	if (showHelpMenu)
	{
		if (ImGui::Begin("About Ragnar Engine", &showHelpMenu))
		{
			ImGui::TextWrapped
			(
				"This is a videogame engine created for our game engine project."
				" We are two students, Lucas Perez and Oriol Bernal, and the final goal for this engine is"
				" to create a videogame."
			);
		}
		ImGui::End();
	}

	return true;
}