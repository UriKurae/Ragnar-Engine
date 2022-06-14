#pragma once
#include "SDL_stdinc.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();

	Uint32 Read();

private:
	
	bool	running;
	Uint32	startedAt;
	Uint32	stoppedAt;
};