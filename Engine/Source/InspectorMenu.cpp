#include "InspectorMenu.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"

#include "GameObject.h"

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
		if (app->editor->GetSelected())
		{
			//ImGui::SetNextTreeNodeOpen(true);
			// This line below draws in the inspector only the selected object, Unity style
			app->editor->GetSelected()->DrawEditor();


			// TODO: The line below is not necessary anymore, it's "HardCoded", does not function like unity's inspector
			//object.DrawEditor();
		}

		//	ImGui::EndMenu();
		//}
	}
	ImGui::End();

	return true;
}