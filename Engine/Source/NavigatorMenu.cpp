#include "Application.h"
#include "NavigatorMenu.h"
#include "ModuleScene.h"

#include "IconsFontAwesome5.h"

#include "Glew/include/GL/glew.h"

NavigatorMenu::NavigatorMenu() : Menu(true)
{
}
NavigatorMenu::~NavigatorMenu()
{
}

bool NavigatorMenu::Update(float dt)
{
	float test = 0;
	ImGui::Begin(ICON_FA_WALKING" Navigator", &active);
	if (ImGui::Button("Bake"))
	{

	}

	ImGui::Text("Agent Properties");
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::DragFloat("Agent Radius", &test, 0.900f);
	ImGui::DragFloat("Agent Height", &test, 2.0f);
	ImGui::DragFloat("Max Slope", &test, 45.0f);
	ImGui::DragFloat("Step Height", &test, 0.5f);

	ImGui::End();
	return true;
}