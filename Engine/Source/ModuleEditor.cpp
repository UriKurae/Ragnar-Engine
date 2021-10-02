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

	console = new ConsoleMenu();

	showMenu = false;
	showHelpMenu = false;
	showConfiguration = true;
	showConsole = true;
	openOptions = false;

	activeInput = true;
	activeWindow = true;
	activeHardware = false;

	memoryCount = 0;
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
			if (ImGui::MenuItem("Exit", "ESC", &ret))
			{
				return false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the file menu");
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("Undo", "Ctrl + Z", &ret);
			ImGui::MenuItem("Redo", "Ctrl + Y", &ret);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("Console", NULL, &console->GetActive());
			ImGui::MenuItem("Configuration", NULL, &showConfiguration);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Depth Test", NULL, app->renderer3D->GetDepthTest()))
			{
				app->renderer3D->SetDepthTest();
			}
			if (ImGui::MenuItem("Cull Face", NULL, app->renderer3D->GetCullFace()))
			{
				app->renderer3D->SetCullFace();
			}
			if (ImGui::MenuItem("Lighting", NULL, app->renderer3D->GetLighting()))
			{
				app->renderer3D->SetLighting();
			}
			if (ImGui::MenuItem("Color Material", NULL, app->renderer3D->GetColorMaterial()))
			{
				app->renderer3D->SetColorMaterial();
			}
			if (ImGui::MenuItem("Texture 2D", NULL, app->renderer3D->GetTexture2D()))
			{
				app->renderer3D->SetTexture2D();
			}
			if (ImGui::MenuItem("Stencil", NULL, app->renderer3D->GetStencil()))
			{
				app->renderer3D->SetStencil();
			}
			if (ImGui::MenuItem("Blending", NULL, app->renderer3D->GetBlending()))
			{
				app->renderer3D->SetBlending();
			}
			if (ImGui::MenuItem("Wire", NULL, app->renderer3D->GetWireMode()))
			{
				app->renderer3D->SetWireMode();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("Demo Menu", NULL, &showMenu);
			ImGui::MenuItem("About Ragnar Engine", "", &showHelpMenu);
			if (ImGui::MenuItem("Documentation", "F1", &ret))
			{
				app->RequestBrowser("https://github.com/UriKurae/Ragnar-Engine");
			}
			if (ImGui::MenuItem("Report a Bug", "", &ret))
			{
				app->RequestBrowser("https://github.com/UriKurae/Ragnar-Engine/issues");
			}
			if (ImGui::MenuItem("Download latest", "", &ret))
			{
				app->RequestBrowser("https://github.com/UriKurae/Ragnar-Engine/releases");
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	if (showMenu)
	{
		ImGui::ShowDemoWindow(&showMenu);
		ImGui::ShowMetricsWindow(&showMenu);

	/*	ImGui::Begin("Ragnar Engine", &showMenu);
		if (ImGui::Button("Close", ImVec2(0, 0)))
		{
			return false;
		}
		ImGui::End();*/

		//ImGui::Button("File", ImVec2(0, 0));
		//ImGui::Button("Edit", ImVec2(0, 0));

		//if (ImGui::CollapsingHeader("Tab"))
		//{
		//	ImGui::TreeNode("Close");
		//}
	}
	static sMStats stats = m_getMemoryStatistics();
	++memoryCount;
	if (memoryCount > 15)
	{
		memoryCount = 0;
		if (memory.size() >= 100)
		{
			for (int i = 0; i < memory.size() - 1; ++i)
			{
				memory[i] = memory[i + 1];
			}
			memory[memory.size() - 1] = (float)stats.totalReportedMemory;
		}
		else
		{
			memory.push_back((float)stats.totalReportedMemory);
		}
	}

	if (fps.size() >= 100)
	{
		for (int i = 0; i < fps.size() - 1; ++i)
		{
			fps[i] = fps[i + 1];
			ms[i] = ms[i + 1];
		}
		fps[fps.size() - 1] = app->GetFPSLimit();
		ms[ms.size() - 1] = 1000.0f / app->GetFPSLimit();
	}
	else
	{
		fps.emplace_back(app->GetFPSLimit());
		ms.emplace_back(1000.0f / app->GetFPSLimit());
	}

	bool r = true;

	if (showConfiguration)
	{
		ImGui::Begin("Configuration", &showConfiguration);

		if (ImGui::BeginMenu("Options", &openOptions))
		{
			if (ImGui::MenuItem("Load"))
			{
				app->LoadConfigRequest();
			}
			if (ImGui::MenuItem("Save"))
			{
				app->SaveConfigRequest();
			}
			ImGui::EndMenu();
		}
		if (ImGui::CollapsingHeader("Application"))
		{
			char* name = "Ragnar Engine";
			ImGui::InputText("App name", name, 25);
			char* organization = "UPC CITM";
			ImGui::InputText("Organization", organization, 25);
			
			int framerate = app->GetFPSLimit();

			if (ImGui::SliderInt("Max FPS", &framerate, 1, 144))
			{
				app->SetFPSLimit(framerate);
			}

			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", fps[fps.size() - 1]);
			ImGui::PlotHistogram("##framerate", &fps[0], fps.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			
			sprintf_s(title, 25, "Milliseconds %0.1f", ms[ms.size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &ms[0], ms.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
			
			ImGui::PlotHistogram("##memory", &memory[0], memory.size(), 0, "Memory Consumption", 0.0f, (float)stats.peakReportedMemory * 1.2f, ImVec2(310, 100));
			
			ImGui::Text("Total Reported Mem: %d", m_getMemoryStatistics().totalReportedMemory);
			ImGui::Text("Total Actual Mem: %d", m_getMemoryStatistics().totalActualMemory);
			ImGui::Text("Peak Reported Mem: %d", m_getMemoryStatistics().peakReportedMemory);
			ImGui::Text("Peak Actual Mem: %d", m_getMemoryStatistics().peakActualMemory);
			ImGui::Text("Accumulated Reported Mem: %d", m_getMemoryStatistics().accumulatedReportedMemory);
			ImGui::Text("Accumulated Actual Mem: %d", m_getMemoryStatistics().accumulatedActualMemory);
			ImGui::Text("Accumulated Alloc Unit Count: %d", m_getMemoryStatistics().accumulatedAllocUnitCount);
			ImGui::Text("Total Alloc Unit Count: %d", m_getMemoryStatistics().totalAllocUnitCount);
			ImGui::Text("Peak Alloc Unit Count: %d", m_getMemoryStatistics().peakAllocUnitCount);
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			ImGui::Checkbox("Active", &activeWindow);
			if (ImGui::SliderFloat("Brightness", app->window->GetWindowBrightness(), 0.0f, 1.0f, "%f", 0))
			{
				app->window->SetBrightness();
			}

			if (ImGui::SliderInt("Width", app->window->GetWindowWidth(), 640, 1920, "%d", 0))
			{
				app->window->SetWindowSize();
			}

			if (ImGui::SliderInt("Height", app->window->GetWindowHeight(), 480, 1080, "%d", 0))
			{
				app->window->SetWindowSize();
			}

			ImGui::Text("Refresh Rate:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", app->window->GetRefreshRate());

			if (ImGui::Checkbox("Fullscreen", app->window->GetWindowFullscreen()))
			{
				app->window->SetFullscreen();
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", app->window->GetWindowResizable()))
			{
				app->window->SetResizable();
			}
			if (ImGui::Checkbox("Borderless", app->window->GetWindowBorderless()))
			{
				app->window->SetBorderless();
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Full desktop", app->window->GetWindowFullscreenDesktop()))
			{
				app->window->SetFullscreenDesktop();
			}
		}
		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Checkbox("Active", &activeInput);
			ImGui::Text("Mouse position : ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d %d", app->input->GetMouseX(), app->input->GetMouseY());

			ImGui::Text("Mouse motion : ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d %d", app->input->GetMouseXMotion(), app->input->GetMouseYMotion());

			ImGui::Text("Mouse wheel : ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", app->input->GetMouseZ());

			ImGui::Separator();

			ImGui::BeginChild("Input");

			for (int i = 0; i < app->input->GetInputList().size(); ++i)
			{
				ImGui::Text(app->input->GetInputList()[i].c_str());
			}

			ImGui::EndChild();
		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Checkbox("Active", &activeHardware);
			SDL_version version;
			SDL_GetVersion(&version);
			ImGui::Text("SDL Version:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.%d.%d", version.major, version.minor, version.patch);
			ImGui::Separator();
			ImGui::Text("CPU:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d(Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
			ImGui::Text("System RAM:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.1f", (float)(SDL_GetSystemRAM() / 1024));
			ImGui::Text("Caps: ");

			if (SDL_HasRDTSC() == SDL_bool::SDL_TRUE)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "RDTSC,");
			}

			if (SDL_HasMMX() == SDL_bool::SDL_TRUE)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "MMX,");
			}

			if (SDL_HasSSE() == SDL_bool::SDL_TRUE)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE,");
			}

			if (SDL_HasSSE2() == SDL_bool::SDL_TRUE)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE2,");
			}

			if (SDL_HasSSE3() == SDL_bool::SDL_TRUE)
			{
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE3,");
			}

			if (SDL_HasSSE41() == SDL_bool::SDL_TRUE)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE41,");
			}

			if (SDL_HasSSE42() == SDL_bool::SDL_TRUE)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE41,");
			}

			if (SDL_HasAVX() == SDL_bool::SDL_TRUE)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "AVX,");
			}

			if (SDL_HasAVX2() == SDL_bool::SDL_TRUE)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "AVX2");
			}

			ImGui::Separator();

			const GLubyte* vendor = glGetString(GL_VENDOR);
			const GLubyte* renderer = glGetString(GL_RENDERER);

			ImGui::Text("GPU:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", vendor);

			ImGui::Text("Brand:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", renderer);

			GLint parameter = 0;
			ImGui::Text("VRAM budget:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%dMB", parameter / 1024);

			ImGui::Text("VRAM usage:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%dMB", parameter / 1024);

			glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &parameter);
			ImGui::Text("VRAM available:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%dMB", parameter / 1024);

			ImGui::Text("VRAM reserved:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%dMB", parameter / 1024);
		}
		ImGui::End();
	}

	if (showConsole)
	{
		console->Update();
	}

	if (app->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		app->window->SetFullscreen();
	}

	if (showHelpMenu)
	{
		if (ImGui::Begin("About Ragnar Engine", &showHelpMenu))
		{
			ImGui::TextWrapped("Ragnar Engine v0.3");
			ImGui::NewLine();
			ImGui::TextWrapped
			(" This is a videogame engine created for our game engine project. We are two students, Lucas Perez and Oriol Bernal, and the final goal for this engine is"
				" to create a videogame."
			);
			ImGui::TextWrapped("By Oriol Bernal & Lucas Perez");
			ImGui::NewLine();

			ImGui::TextWrapped("3rd Party Libraries used:");
			SDL_version sdlVer;
			SDL_GetVersion(&sdlVer);
			ImGui::TextWrapped("-SDL %d.%d.%d", sdlVer.major, sdlVer.minor, sdlVer.patch);
			ImGui::TextWrapped("-Glew %s", glewGetString(GLEW_VERSION));
			ImGui::TextWrapped("-ImGui %s", ImGui::GetVersion());
			ImGui::TextWrapped("-MathGeoLib 1.5");
			ImGui::TextWrapped("-OpenGl version %s", glGetString(GL_VERSION));

			ImGui::NewLine();
			ImGui::TextWrapped("License:");
			ImGui::NewLine();
			ImGui::TextWrapped("MIT License");
			ImGui::NewLine();
			ImGui::TextWrapped("Copyright(c) 2021 Oriol Bernal Martinez & Lucas Perez Garcia");
			ImGui::NewLine();
			ImGui::TextWrapped(
				"Permission is hereby granted, free of charge, to any person obtaining a copy"
				"of this softwareand associated documentation files(the 'Software'), to deal"
				"in the Software without restriction, including without limitation the rights"
				"to use, copy, modify, merge, publish, distribute, sublicense, and /or sell"
				"copies of the Software, and to permit persons to whom the Software is"
				"furnished to do so, subject to the following conditions : "
			);
			ImGui::NewLine();
			ImGui::TextWrapped(
				"The above copyright noticeand this permission notice shall be included in all"
				"copies or substantial portions of the Software."
			);
			ImGui::NewLine();
			ImGui::TextWrapped(
				"THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR"
				"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,"
				"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE"
				"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER"
				"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,"
				"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE"
				"SOFTWARE."
			);
		}
		ImGui::End();
	}

	return true;
}

bool ModuleEditor::Draw()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return true;
}

bool ModuleEditor::CleanUp()
{
	RELEASE(console);

	fps.clear();
	ms.clear();
	memory.clear();

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
	if (console) console->AddLog(string);
}