#include "timer.h"
#include <time.h>
#include <windows.h>

timerclass::timerclass()
{
	scalar = 0.0f;
	frequency = 0;
	resolution = 0.0f;
	mm_timer_start = 0;
	mm_timer_elapsed = 0;
	performance_timer = 0.0f;
	performance_timer_start = 0;
	performance_timer_elapsed = 0;
}

int timerclass::ShowFps()
{
	static int realfps = 0;
	static int fps = 0;
	static float timerclock = 0;
	if (timerclock < 4.0f)
	{
		timerclock+=scalar;
		fps++;
	}
	else
	{
		realfps = fps/4;
		timerclock = 0.0f;
		fps = 0;
	}
	return realfps;
}
