#pragma once

#include "Menu.h"

class InputActionMenu : Menu
{
public:
	InputActionMenu(); 
	~InputActionMenu();

	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

private:

};