#include "Application.h"
#include "Globals.h"
#include "ModuleInput.h"
#include "FileSystem.h"
#include "ModuleEditor.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"

#include "Profiling.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(bool startEnabled) : Module(startEnabled)
{
	name = "Input";
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, (int)KeyState::KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	repeated = false;
}

// Destructor
ModuleInput::~ModuleInput()
{
	RELEASE_ARRAY(keyboard);
}

// Called before render is available
bool ModuleInput::Init(JsonParsing& node)
{
	DEBUG_LOG("Init SDL input event system");

	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		DEBUG_LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	return ret;
}

// Called every draw update
bool ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		std::string string;
		if(keys[i] == 1)
		{
			if (keyboard[i] == KeyState::KEY_IDLE)
			{
				keyboard[i] = KeyState::KEY_DOWN;
				string = "Keybr :" + std::to_string(i) + " - DOWN";
				strings.push_back(string);
			}
			else
			{
				keyboard[i] = KeyState::KEY_REPEAT;
				if (!repeated)
				{
					repeated = true;
					string = "Keybr :" + std::to_string(i) + " - REPEAT";
					strings.push_back(string);
				}
			}
		}
		else
		{
			if (keyboard[i] == KeyState::KEY_REPEAT || keyboard[i] == KeyState::KEY_DOWN)
			{
				repeated = false;
				keyboard[i] = KeyState::KEY_UP;
				string = "Keybr :" + std::to_string(i) + " - UP";
				strings.push_back(string);
			}
			else
				keyboard[i] = KeyState::KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

	mouseX /= SCREEN_SIZE;
	mouseY /= SCREEN_SIZE;
	mouseZ = 0;

	for(int i = 0; i < 5; ++i)
	{
		std::string string;
		if(buttons & SDL_BUTTON(i))
		{
			if (mouseButtons[i] == KeyState::KEY_IDLE)
			{
				mouseButtons[i] = KeyState::KEY_DOWN;
				string = "Mouse :" + std::to_string(i) + " - DOWN";
				strings.push_back(string);
			}
			else
			{
				mouseButtons[i] = KeyState::KEY_REPEAT;
				if (!repeated)
				{
					repeated = true;
					string = "Mouse :" + std::to_string(i) + " - REPEAT";
					strings.push_back(string);
				}
			}
		}
		else
		{
			if (mouseButtons[i] == KeyState::KEY_REPEAT || mouseButtons[i] == KeyState::KEY_DOWN)
			{
				repeated = false;
				mouseButtons[i] = KeyState::KEY_UP;
				string = "Mouse :" + std::to_string(i) + " - UP";
				strings.push_back(string);
			}
			else
			{
				mouseButtons[i] = KeyState::KEY_IDLE;
			}
		}
	}

	mouseXMotion = mouseYMotion = 0;

	bool quit = false;
	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouseZ = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouseX = e.motion.x / SCREEN_SIZE;
			mouseY = e.motion.y / SCREEN_SIZE;

			mouseXMotion = e.motion.xrel / SCREEN_SIZE;
			mouseYMotion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_DROPFILE:
			{
				std::string filePath = e.drop.file;
				app->fs->ImportFromOutside(filePath, app->editor->GetCurrentDir());
				SDL_free(&filePath);
			}	
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					app->ResizeRequest();
			}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KeyState::KEY_UP)
		return false;

	return true;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	DEBUG_LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

bool ModuleInput::LoadConfig(JsonParsing& node)
{
	return true;
}

bool ModuleInput::SaveConfig(JsonParsing& node) const
{
	return true;
}
