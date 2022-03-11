#include "Application.h"
#include "Globals.h"

#include "NavigatorMenu.h"
#include "InputGeom.h"
#include "NavMeshBuilder.h"

#include "IconsFontAwesome5.h"

#include "Glew/include/GL/glew.h"

NavigatorMenu::NavigatorMenu() : Menu(true)
{
}

NavigatorMenu::~NavigatorMenu()
{
}

bool NavigatorMenu::Start()
{
    buildSettings = app->navMesh->GetBuildSettings();

    DEBUG_LOG("Linking the ModuleNavMesh Settings");
    bool ret = true;

    return ret;
}

bool NavigatorMenu::Update(float dt)
{
    ImGui::Begin(ICON_FA_WALKING" Navigator", &active);
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x - 180) / 2);
    if (ImGui::Button("Bake NavMesh", ImVec2(180.0f, 20.0f)))
    {
        app->navMesh->BakeNavMesh();
    }
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Agent Properties");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::PushItemWidth(180);
    ImGui::DragFloat("Agent radius", &buildSettings->agentRadius, 0.1f);
    ImGui::DragFloat("Agent height", &buildSettings->agentHeight, 0.1f);
    ImGui::DragFloat("Agent max climb", &buildSettings->agentMaxClimb, 0.1f);
    ImGui::DragFloat("Agent max slope", &buildSettings->agentMaxSlope, 0.1f);
    ImGui::PopItemWidth();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("NavMesh Properties");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::PushItemWidth(180);
    ImGui::DragFloat("Cell size", &buildSettings->cellSize, 0.1f);
    ImGui::DragFloat("Cell hight", &buildSettings->cellHeight, 0.1f);
    ImGui::DragFloat("Region minimum size", &buildSettings->regionMinSize, 0.1f);
    ImGui::DragFloat("Region merge size", &buildSettings->regionMergeSize, 0.1f);
    ImGui::DragFloat("Edge max lenght", &buildSettings->edgeMaxLen, 0.1f);
    ImGui::DragFloat("Edge max error", &buildSettings->edgeMaxError, 0.1f);
    ImGui::DragFloat("Detail sample distance", &buildSettings->detailSampleDist, 0.1f);
    ImGui::DragFloat("Detail sample max error", &buildSettings->detailSampleMaxError, 0.1f);
    ImGui::DragFloat("Tile Size", &buildSettings->tileSize, 0.1f);
    ImGui::PopItemWidth();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("NavMeshBuilder"))
    {
        ImGui::Text("NaveMesh Geometry");
        ImGui::Separator();
        ImGui::Spacing();
        
        const SimpleMesh* mesh = app->navMesh->GetInputGeom()->getMesh();
        const NavMeshBuilder* builder = app->navMesh->GetNavMeshBuilder();
        if (mesh->vertices.size() <= 0 && mesh->indices.size() <= 0)
            ImGui::Text("No Geometry added");
        else 
        {
            ImGui::Text("Verts: %d", mesh->vertices.size());
            ImGui::Text("Indices: %d", mesh->indices.size());

            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::Text("Max Tiles: %i", builder->GetMaxTyles());
            ImGui::Text("Max Polys per tiles: %i", builder->GetMaxPolyTile());
            ImGui::Text("Cell size: %.4f", buildSettings->cellSize);
            ImGui::Text("Cell Height: %.4f", buildSettings->cellHeight);

            if (builder->GetPolyMesh() != nullptr)
            {
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::Text("PolyMesh");

                ImGui::Text("Verts: %i", builder->GetPolyMesh()->nverts);
                ImGui::Text("Polys: %i", builder->GetPolyMesh()->npolys);
            }
        }
    }

    ImGui::End();
    return true;
}