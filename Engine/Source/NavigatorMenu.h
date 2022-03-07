#pragma once
#include "Menu.h"
#include "ModuleNavMesh.h"

struct BuildSettings;

class NavigatorMenu : public Menu
{
public:
    NavigatorMenu();
    ~NavigatorMenu();

    bool Update(float dt) override;
private:
    BuildSettings settings;
};