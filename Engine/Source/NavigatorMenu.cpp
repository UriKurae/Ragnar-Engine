#include "Application.h"
#include "NavigatorMenu.h"
#include "ModuleScene.h"
#include "ModuleNavMesh.h"

#include "IconsFontAwesome5.h"

#include "Glew/include/GL/glew.h"

NavigatorMenu::NavigatorMenu() : Menu(true)
{
    settings.cellSize               = 0.3f;
    settings.cellHeight             = 0.2f;
    settings.agentHeight            = 2.0f;
    settings.agentRadius            = 0.6f;
    settings.agentMaxClimb          = 0.9f;
    settings.agentMaxSlope          = 45.0f;
    settings.regionMinSize          = 8;
    settings.regionMergeSize        = 20;
    settings.edgeMaxLen             = 12.0f;
    settings.edgeMaxError           = 1.3f;
    settings.vertsPerPoly           = 6.0f;
    settings.detailSampleDist       = 6.0f;
    settings.detailSampleMaxError   = 1.0f;
    settings.partitionType          = 0.0f;
    settings.tileSize               = 0.0f;
}

NavigatorMenu::~NavigatorMenu()
{
}

bool NavigatorMenu::Update(float dt)
{
    ImGui::Begin(ICON_FA_WALKING" Navigator", &active);
    if (ImGui::Button("Bake"))
    {
        //app->navMesh->SetGeometry(settings);
        app->navMesh->BakeNavMesh();
    }
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Agent Properties");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::DragFloat("Agent radius", &settings.agentRadius, 0.900f);
    ImGui::DragFloat("Agent height", &settings.agentHeight, 2.0f);
    ImGui::DragFloat("Agent max climb", &settings.agentMaxClimb, 2.0f);
    ImGui::DragFloat("Agent max slope", &settings.agentMaxSlope, 2.0f);
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("NavMesh Properties");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::DragFloat("Cell size", &settings.cellSize, 2.0f);
    ImGui::DragFloat("Cell hight", &settings.cellHeight, 2.0f);
    ImGui::DragFloat("Region minimum size", &settings.regionMinSize, 2.0f);
    ImGui::DragFloat("Region merge size", &settings.regionMergeSize, 2.0f);
    ImGui::DragFloat("Edge max lenght", &settings.edgeMaxLen, 2.0f);
    ImGui::DragFloat("Edge max error", &settings.edgeMaxError, 2.0f);
    ImGui::DragFloat("Detail sample distance", &settings.detailSampleDist, 2.0f);
    ImGui::DragFloat("Detail sample max error", &settings.detailSampleMaxError, 2.0f);
    ImGui::DragFloat("Tile Size", &settings.tileSize, 0.0f);

    ImGui::End();
    return true;
}