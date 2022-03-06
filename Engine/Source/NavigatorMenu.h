#pragma once
#include "Menu.h"
#include "InputGeom.h"

class NavigatorMenu : public Menu
{
public:
    NavigatorMenu();
    ~NavigatorMenu();

    bool Update(float dt) override;
private:
    BuildSettings settings;
};