#include "ModuleEditor.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"

#include "ConsoleMenu.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>

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
	bool ret = true;

	ret = mainMenuBar.Update(dt);

	if (app->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		app->window->SetFullscreen();
	}

	return ret;
}

bool ModuleEditor::Draw()
{
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