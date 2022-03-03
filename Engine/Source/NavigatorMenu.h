#pragma once
#include "Menu.h"

class NavigatorMenu : public Menu
{
public:
	NavigatorMenu();
	~NavigatorMenu();

	bool Update(float dt) override;
};
