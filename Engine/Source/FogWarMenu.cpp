#include "FogWarMenu.h"
#include "Application.h"

#include "ModuleSceneManager.h"
#include "Scene.h"
#include "GL/glew.h"

#include "Profiling.h"

FogWarMenu::FogWarMenu() : Menu(true, "Fog War")
{
}
FogWarMenu::~FogWarMenu()
{
}

bool FogWarMenu::Update(float dt)
{
	RG_PROFILING_FUNCTION("FogOfWar Menu Update");

	if (ImGui::Begin(ICON_FA_CLOUD" Fog War", &active))
	{
		Frustum* frustum = app->sceneManager->GetCurrentScene()->mainCamera->GetFrustum();
		if (ImGui::Checkbox("Active", &fog))
		{
			if (fog)
			{
				glEnable(GL_FOG);
				glFogfv(GL_FOG_COLOR, fogColor); // Set the fog color
				if (fogLinear)
				{
					glFogi(GL_FOG_MODE, GL_LINEAR); // GL_LINEAR constant is an integer.
					glFogf(GL_FOG_START, fogStart);
					glFogf(GL_FOG_END, fogEnd);
				}
				else
				{
					glFogi(GL_FOG_MODE, GL_EXP);
					glFogf(GL_FOG_DENSITY, fogDensity);
				}

				// Set far plane nearer the camera when fog is active
				float newFarPlane = frustum->FarPlaneDistance();
				oldFarPlane = newFarPlane;
				newFarPlane = fogFarPlane;
				frustum->SetViewPlaneDistances(frustum->NearPlaneDistance(), fogFarPlane);
				//gluPerspective(45.0f, 800.0f / 600.0f, 1.0f, newFarPlane);
			}
			else
			{
				glDisable(GL_FOG);
				frustum->SetViewPlaneDistances(frustum->NearPlaneDistance(), oldFarPlane); // Reset camera far plane
			}
		}

		if (frustum->FarPlaneDistance() != fogFarPlane)
			fogFarPlane = frustum->FarPlaneDistance(); // Not very efficient

		if (ImGui::CollapsingHeader("Custom", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Checkbox("Linear", &fogLinear) && fogExpo)
			{
				glFogi(GL_FOG_MODE, GL_LINEAR);
				fogExpo = false;
				fogLinear = true;
			}

			if (ImGui::Checkbox("Exponential", &fogExpo) && fogLinear)
			{
				glFogi(GL_FOG_MODE, GL_EXP);
				fogLinear = false;
				fogExpo = true;
			}

			if (ImGui::SliderFloat("Red", &fogColor[0], 0.0f, 1.0f))
				glFogfv(GL_FOG_COLOR, fogColor); // Set the fog color

			if (ImGui::SliderFloat("Green", &fogColor[1], 0.0f, 1.0f));
			glFogfv(GL_FOG_COLOR, fogColor);

			if (ImGui::SliderFloat("Blue", &fogColor[2], 0.0f, 1.0f));
			glFogfv(GL_FOG_COLOR, fogColor);

			if (fogLinear)
			{
				if (ImGui::SliderFloat("Start", &fogStart, 10.0f, 100.0f))
					if (fogEnd < fogStart) fogEnd = fogStart + 1;
				glFogf(GL_FOG_START, fogStart);

				if (ImGui::SliderFloat("End", &fogEnd, fogStart+1, 1000.0f));
				glFogf(GL_FOG_END, fogEnd);
			}

			else if (ImGui::SliderFloat("Density", &fogDensity, 0.0f, 1.0f));
			glFogf(GL_FOG_DENSITY, fogDensity);
		}
	}

	ImGui::End();
	return true;
}