#include "Application.h"
#include "Globals.h"

#include "ModuleWindow.h"

#include "Profiling.h"

ModuleWindow::ModuleWindow(bool startEnabled) : Module(startEnabled)
{
	name = "Window";
	window = NULL;
	screenSurface = NULL;

	fullscreen = false;
	resizable = false;
	borderless = true;
	fullscreenDesktop = false;

	width = 0;
	height = 0;
	refreshRate = 0;

	brightness = 0;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(JsonParsing& node)
{
	DEBUG_LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		DEBUG_LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = (int)node.GetJsonNumber("width") * SCREEN_SIZE;
		height = (int)node.GetJsonNumber("height") * SCREEN_SIZE;
		brightness = (float)node.GetJsonNumber("brightness");

		fullscreen = node.GetJsonBool("fullscreen");
		fullscreenDesktop = node.GetJsonBool("fullscreen desktop");
		borderless = node.GetJsonBool("borderless");
		
		//Use OpenGL 3.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		if(fullscreen)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(borderless)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(fullscreenDesktop)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			DEBUG_LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);
			SDL_SetWindowBrightness(window, brightness);
			SDL_DisplayMode* display = new SDL_DisplayMode();
			SDL_GetWindowDisplayMode(window, display);
			refreshRate = display->refresh_rate;
			RELEASE(display);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	DEBUG_LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

bool ModuleWindow::SaveConfig(JsonParsing& node) const
{
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "width", width);
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "height", height);
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "brightness", brightness);

	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "fullscreen", fullscreen);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "fullscreen desktop", fullscreenDesktop);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "borderless", borderless);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "resizable", resizable);
	
	return true;
}

bool ModuleWindow::LoadConfig(JsonParsing& node)
{
	width = (int)node.GetJsonNumber("width") * SCREEN_SIZE;
	height = (int)node.GetJsonNumber("height") * SCREEN_SIZE;
	brightness = (float)node.GetJsonNumber("brightness");

	fullscreen = node.GetJsonBool("fullscreen");
	fullscreenDesktop = node.GetJsonBool("fullscreen desktop");
	borderless = node.GetJsonBool("borderless");

	if (fullscreen) SetFullscreen();
	else if (fullscreenDesktop) SetFullscreenDesktop();
	if (borderless) SetBorderless();
	
	SetWindowSize();

	return true;
}

void ModuleWindow::SetTitle(const char* title) const
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetFullscreen() const
{
	if (fullscreen) 
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else 
		SDL_SetWindowFullscreen(window, 0);
}

void ModuleWindow::SetFullscreenDesktop() const
{
	if (fullscreenDesktop) 
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else 
		SDL_SetWindowFullscreen(window, 0);
}

void ModuleWindow::SetResizable() const
{
	// TODO

	/*if (resizable)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_RESIZABLE);
	else
		SDL_SetWindowFullscreen(window, 0);*/
}

void ModuleWindow::SetBorderless() const
{
	SDL_SetWindowBordered(window, (SDL_bool)borderless);
}

void ModuleWindow::SetBrightness() const
{
	SDL_SetWindowBrightness(window, brightness);
}

void ModuleWindow::SetWindowSize() const
{
	SDL_SetWindowSize(window, width, height);
}

int* ModuleWindow::GetWindowWidth()
{
	SDL_GetWindowSize(window, &width, &height);
	return &width;
}

int* ModuleWindow::GetWindowHeight()
{
	SDL_GetWindowSize(window, &width, &height);
	return &height;
}