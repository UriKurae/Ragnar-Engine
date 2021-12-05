#include "InspectorMenu.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"

#include "GameObject.h"
#include "Resource.h"

#include "Profiling.h"

InspectorMenu::InspectorMenu() : Menu(true)
{
}

InspectorMenu::~InspectorMenu()
{
}

bool InspectorMenu::Update(float dt)
{
	ImGui::Begin("Inspector", &active);
	if (!app->scene->GetRoot()->GetChilds().empty())
	{
		//GameObject& object = (*app->scene->GetGameObjectsList()[0]);

		// TODO: Uncomment if you want
		/*if (ImGui::BeginMenu(object.GetName()))
		{*/
		if (app->editor->GetGO())
		{
			//ImGui::SetNextTreeNodeOpen(true);
			// This line below draws in the inspector only the selected object, Unity style
			if (ImGui::BeginChild("Selected GO"))
			{
				app->editor->GetGO()->DrawEditor();
				ImGui::EndChild();
			}

			// TODO: The line below is not necessary anymore, it's "HardCoded", does not function like unity's inspector
			//object.DrawEditor();
		}
		else if (app->editor->GetResource())
		{
			app->editor->GetResource()->DrawOnEditor();
		}

		//	ImGui::EndMenu();
		//}
	}
	ImGui::End();

	return true;
}