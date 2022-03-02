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
	ImGui::Begin(ICON_FA_WALKING" Navigator", &active);
	if (ImGui::Button("Bake"))
	{

	}

	ImGui::End();
	return true;
}