#pragma once

#include <SDL.h>

class Timer
{
public:
	Timer();

	void restart();

	Uint32 getTicks();

private:
	Uint32 startTicks;
};

