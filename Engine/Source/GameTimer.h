#pragma once
#include "JsonParsing.h"
#include <SDL/include/SDL_timer.h>

class GameTimer
{
public:
	GameTimer();
	~GameTimer();

	void Start();
	void Stop();
	void FinishUpdate();

	inline void SetDesiredCappedMs(int miliseconds) { cappedMs = miliseconds; }
	inline void SetDesiredDeltaTime(float dt) { deltaTime = dt; }
	void SetTimeScale(float scale) { timeScale = scale; }

	int GetEngineTimeStartup() const;
	int GetTime() const;
	inline int GetFramesSinceStartup() const { return frameCounter; }
	inline float GetDeltaTime() const { return deltaTime * timeScale; }
	inline int GetFps() const { return (1.0f / ((float)cappedMs) * 1000.0f); }
	inline float GetTimeScale() const { return timeScale; }
	

	Uint32 ReadTime();
	void ResetTimer();

	void ReadConfig(JsonParsing& node);
	void SaveConfig(JsonParsing& node);

public:

	bool isRunning;

private:
	// Normal timings
	int timer, stopped_at;
	float deltaTime;
	int frameCounter;
	int lastFrameMs;
	int cappedMs;
	float timeScale;

	// Engine timings
	int gameTimer;
	bool gameStarted;

	// Json Parsing for loading and saving things
	JsonParsing jsonFile;
};