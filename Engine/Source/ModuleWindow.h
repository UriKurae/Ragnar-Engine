#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool startEnabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title) const;
	void SetFullscreen() const;
	void SetFullscreenDesktop() const;
	void SetResizable() const;
	void SetBorderless() const;
	void SetBrightness() const;
	void SetWindowSize() const;

	float* GetWindowBrightness();
	int* GetWindowWidth();
	int* GetWindowHeight();
	bool* GetWindowFullscreen();
	bool* GetWindowFullscreenDesktop();
	bool* GetWindowResizable();
	bool* GetWindowBorderless();
	int GetRefreshRate() const;


public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screenSurface;

	bool fullscreen;
	bool fullscreenDesktop;
	bool resizable;
	bool borderless;

	int width;
	int height;
	int refreshRate;

	float brightness;
};

#endif // __ModuleWindow_H__