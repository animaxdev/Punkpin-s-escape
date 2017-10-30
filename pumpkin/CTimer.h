#pragma once

#include <SDL.h>
//#include "CModel.h"
#include <functional>

class CTimer
{
private:
	Uint32 startTicks;
	Uint32 pausedTicks;
	bool paused;
	bool started;


public:
	CTimer() {
		paused = false;
		started = false;
		startTicks = 0;
		pausedTicks = 0;
	}

	~CTimer() { }

	static CTimer& getInstance();
	void start();
	void pause();
	void unpause();

	//	In milliseconds
	//
	Uint32 getTicks();

	//	Sets up "iter"-times looped function after given callback time (min. 200ms)
	//
	void setCallback(Uint32 time, std::function<void()> fptr, unsigned iter = 1);
	bool timeElapsed(Uint32 startTime, Uint32 count)
	{
		return ((startTime + getTicks()) % count < 20);
	}
	bool isStarted() const { return started; }
	bool isPaused() const { return paused; }
};