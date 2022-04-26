#include "GameTimer.h"
#include "SDL_timer.h"

GameTimer::GameTimer() : gameTimer(0.0f), gameStarted(false), deltaTime(0.016f), frameCounter(0), timer(0.0f), lastFrameMs(0.0f), cappedMs(0.0f), timeScale(1.0f)
{
	gameTimer = SDL_GetTicks();
}

GameTimer::~GameTimer()
{
}

void GameTimer::Start()
{
	isRunning = true;
	frameCounter++;
	/*deltaTime = (float)GetTime() / 1000.0f;*/
	timer = SDL_GetTicks();
}

void GameTimer::Stop()
{
	isRunning = false;
	stopped_at = SDL_GetTicks();
}

void GameTimer::FinishUpdate()
{
	lastFrameMs = GetTime();
	if ((cappedMs > 0) && (lastFrameMs < cappedMs)) SDL_Delay(cappedMs - lastFrameMs);
}

int GameTimer::GetEngineTimeStartup() const
{
	return (SDL_GetTicks() - gameTimer);
}

int GameTimer::GetTime() const
{
	return (SDL_GetTicks() - timer);
}


Uint32 GameTimer::ReadTime()
{
	if (isRunning == true)
	{
		return SDL_GetTicks() - timer;
	}
	else
	{
		return stopped_at - timer;
	}
}

void GameTimer::ResetTimer()
{
	gameTimer = SDL_GetTicks();
}

void GameTimer::ReadConfig(JsonParsing& node)
{
	cappedMs = (1 / node.GetJsonNumber("gameFPS")) * 1000;
	int a = 0;
	a += 5;
}

void GameTimer::SaveConfig(JsonParsing& node)
{
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "gameFPS", GetFps());
}
