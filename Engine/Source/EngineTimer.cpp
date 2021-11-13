#include "EngineTimer.h"

#include "SDL\include\SDL.h"


EngineTimer::EngineTimer() : engineTimer(0.0f), engineStarted(false), deltaTime(0.0f), frameCounter(0), timer(0.0f), lastFrameMs(0.0f), cappedMs(0.0f)
{
	engineTimer = SDL_GetTicks();
	engineStarted = true;
}

EngineTimer::~EngineTimer()
{
}

void EngineTimer::Start()
{
	frameCounter++;
	deltaTime = (float)GetTime() / 1000.0f;
	timer = SDL_GetTicks();
}

void EngineTimer::FinishUpdate()
{
	lastFrameMs = GetTime();

	if ((cappedMs > 0) && (lastFrameMs < cappedMs)) SDL_Delay(cappedMs - lastFrameMs);
}

int EngineTimer::GetEngineTimeStartup() const
{
	return (SDL_GetTicks() - engineTimer);
}

int EngineTimer::GetTime() const
{
	return (SDL_GetTicks() - timer);
}

void EngineTimer::ReadConfig(JsonParsing& node)
{
	cappedMs = (1 / node.GetJsonNumber("FPS")) * 1000;
}

void EngineTimer::SaveConfig(JsonParsing& node)
{
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "FPS", GetFps());
}
