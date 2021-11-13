#pragma once

#include "JsonParsing.h"

class EngineTimer
{
public:
	EngineTimer();
	~EngineTimer();

	void Start();
	void FinishUpdate();

	inline void SetDesiredCappedMs(int miliseconds) { cappedMs = miliseconds; }

	int GetEngineTimeStartup() const;
	int GetTime() const;
	inline int GetFramesSinceStartup() const { return frameCounter; }
	inline float GetDeltaTime() const { return deltaTime; }
	inline int GetFps() const { return (1.0f / ((float)cappedMs) * 1000.0f); }

	void ReadConfig(JsonParsing& node);
	void SaveConfig(JsonParsing& node);
private:
	// Normal timings
	int timer;
	float deltaTime;
	int frameCounter;
	int lastFrameMs;
	int cappedMs;

	// Engine timings
	int engineTimer;
	bool engineStarted;

	// Json Parsing for loading and saving things
	JsonParsing jsonFile;
};