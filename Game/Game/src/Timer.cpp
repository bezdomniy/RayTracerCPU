#include "Timer.h"

Timer::Timer()
{
	startTicks = 0;
}

void Timer::restart()
{
	startTicks = SDL_GetTicks();
}

Uint32 Timer::getTicks()
{
	Uint32 time = 0;

	time = SDL_GetTicks() - startTicks;

	return time;
}
