#include "AboutMenu.h"

#include <SDL_syswm.h>
#include <GL\glew.h>

#include "assimp/version.h"
#include "DevIL/include/IL/il.h"
#include "PhysFS/include/physfs.h"

#include "Profiling.h"

AboutMenu::AboutMenu() : Menu(false)
{
}

AboutMenu::~AboutMenu()
{
}

bool AboutMenu::Update(float dt)
{
	if (ImGui::Begin("About Ragnar Engine", &active))
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
		ImGui::TextWrapped("-Assimp version %d.%d", aiGetVersionMajor(), aiGetVersionMinor());
		ImGui::TextWrapped("-DevIL version 1.8.0");
		ImGui::TextWrapped("-PhysFS version %d.%d.%d", PHYSFS_VER_MAJOR, PHYSFS_VER_MINOR, PHYSFS_VER_PATCH);

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

	return true;
}