#pragma once
#include "Menu.h"

struct BuildSettings;

class NavigatorMenu : public Menu
{
public:
    NavigatorMenu();
    ~NavigatorMenu();

    bool Start();
    bool Update(float dt) override;

private:
    BuildSettings* buildSettings;
};