#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ConfigurationMenu.h"

#include <GL\glew.h>

#include "Profiling.h"

ConfigurationMenu::ConfigurationMenu() : Menu(false)
{
	memoryCount = 0;

	activeWindow = true;
	openOptions = false;
	activeInput = true;
	activeHardware = true;
}

ConfigurationMenu::~ConfigurationMenu()
{
}

bool ConfigurationMenu::Update(float dt)
{
	static sMStats stats = m_getMemoryStatistics();
	++memoryCount;
	if (memoryCount > 15)
	{
		memoryCount = 0;
		if (memory.size() >= 100)
		{
			for (unsigned int i = 0; i < memory.size() - 1; ++i)
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
		for (unsigned int i = 0; i < fps.size() - 1; ++i)
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

	ImGui::Begin("Configuration", &active);

	if (ImGui::BeginMenu("Options"))
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
		ImGui::Text("Ragnar Engine");
		ImGui::Text("UPC CITM");

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
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Text("Field of view");
		ImGui::SameLine();
		static float horizontalFovEditorCamera = RadToDeg(app->camera->horizontalFov);
		if (ImGui::DragFloat("", &horizontalFovEditorCamera, 1.0f, 1.0f, 179.0f))
		{
			app->camera->horizontalFov = DegToRad(horizontalFovEditorCamera);
			app->camera->UpdateFov();
		}

		ImGui::Text("Clipping planes");

		ImGui::Text("Near");
		ImGui::SameLine();
		ImGui::PushID("NearPlane");
		if (ImGui::DragFloat("", &app->camera->nearPlane, 0.5f, 0.1f)) app->camera->SetPlanes();
		ImGui::PopID();

		ImGui::PushID("farPlane");
		ImGui::Text("Far");
		ImGui::SameLine();
		if (ImGui::DragFloat("", &app->camera->farPlane, 0.5f, 0.1f)) app->camera->SetPlanes();
		ImGui::PopID();

		ImGui::Checkbox("Visualize frustum culling on editor", &app->camera->visualizeFrustum);
	}
	ImGui::PopID();
	if (ImGui::CollapsingHeader("Renderer"))
	{
		if (ImGui::Checkbox("VSync", app->renderer3D->GetVsync()))
		{
			app->renderer3D->SetVsync();
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

		for (unsigned int i = 0; i < app->input->GetInputList().size(); ++i)
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

	return true;
}