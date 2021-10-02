#pragma once

#include "Menu.h"

class AboutMenu : public Menu
{
public:
	AboutMenu();
	~AboutMenu();

	bool Update(float dt) override;
};