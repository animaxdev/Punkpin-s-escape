#include "CTimer.h"



CTimer & CTimer::getInstance()
{
    static CTimer *instance = new CTimer();
    return *instance;
}

CTimer::CTimer()
{
    paused = false;
    started = false;
    startTicks = 0;
    pausedTicks = 0;
}

void CTimer::start()
{
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();
    pausedTicks = 0;
}

void CTimer::pause()
{
    paused = true;
    pausedTicks = SDL_GetTicks() - startTicks;
    startTicks = 0;
}

void CTimer::unpause()
{
    if (started && paused)
    {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

Uint32 CTimer::getTicks()
{
    if (started)
    {
        if (paused)
            return pausedTicks;
        else
            return (SDL_GetTicks() - startTicks);
    }
    return 0;
}


void CTimer::setCallback(Uint32 time, std::function<void()> fptr, unsigned iter)
{
    static Uint32 next = time;
    static Uint32 actual;
    actual = getTicks();

    if (actual % time < 200 && actual > next)
    {
        while (iter--)
        {
            fptr();
            next = actual + time;
        }
    }
}
