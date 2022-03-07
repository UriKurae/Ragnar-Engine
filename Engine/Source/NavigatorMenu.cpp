#include "Application.h"
#include "NavigatorMenu.h"

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
        app->navMesh->BakeNavMesh(settings);
    }
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Agent Properties");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::DragFloat("Agent radius", &settings.agentRadius, 0.1f);
    ImGui::DragFloat("Agent height", &settings.agentHeight, 0.1f);
    ImGui::DragFloat("Agent max climb", &settings.agentMaxClimb, 0.1f);
    ImGui::DragFloat("Agent max slope", &settings.agentMaxSlope, 0.1f);
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("NavMesh Properties");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::DragFloat("Cell size", &settings.cellSize, 0.1f);
    ImGui::DragFloat("Cell hight", &settings.cellHeight, 0.1f);
    ImGui::DragFloat("Region minimum size", &settings.regionMinSize, 0.1f);
    ImGui::DragFloat("Region merge size", &settings.regionMergeSize, 0.1f);
    ImGui::DragFloat("Edge max lenght", &settings.edgeMaxLen, 0.1f);
    ImGui::DragFloat("Edge max error", &settings.edgeMaxError, 0.1f);
    ImGui::DragFloat("Detail sample distance", &settings.detailSampleDist, 0.1f);
    ImGui::DragFloat("Detail sample max error", &settings.detailSampleMaxError, 0.1f);
    ImGui::DragFloat("Tile Size", &settings.tileSize, 0.1f);

    ImGui::End();
    return true;
}