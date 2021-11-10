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

ModuleEditor::ModuleEditor() : selected(nullptr), selectedParent(nullptr), Module()
{
	name = "Editor";

	viewport = new Viewport();
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Start()
{
	mainMenuBar.Start();

	return true;
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