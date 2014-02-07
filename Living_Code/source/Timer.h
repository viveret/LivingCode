#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

struct Timer
{
	Timer();

	void Start();
	void End();
	void Update();
	//
	float Next_Poll;//per sec
	unsigned int Collect_Poll,Average;
	float dt;//time
	float Frame_Time;
	LARGE_INTEGER frequency;        // ticks per second
    LARGE_INTEGER Time_Hold;           // ticks



	DWORD next_game_tick;
};

#endif