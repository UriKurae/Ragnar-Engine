#pragma once
#include "Module.h"

typedef struct SDL_Window;
typedef struct SDL_Surface;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool startEnabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(JsonParsing& node) override;
	bool CleanUp();

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) override;

	void SetTitle(const char* title) const;
	void SetFullscreen() const;
	void SetFullscreenDesktop() const;
	void SetResizable() const;
	void SetBorderless() const;
	void SetBrightness() const;
	void SetWindowSize() const;

	inline float* GetWindowBrightness() { return &brightness; }
	int* GetWindowWidth();
	int* GetWindowHeight();
	inline bool* GetWindowFullscreen() { return &fullscreen; }
	inline bool* GetWindowFullscreenDesktop() { return &fullscreenDesktop; }
	inline bool* GetWindowResizable() { return &resizable; }
	inline bool* GetWindowBorderless() { return &borderless; }
	inline int GetRefreshRate() const { return refreshRate; }


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