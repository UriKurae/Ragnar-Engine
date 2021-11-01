#pragma once

#include "Menu.h"

class InspectorMenu : public Menu
{
public:
	InspectorMenu();

	~InspectorMenu();

	bool Update(float dt);
};