#include "Timer.h"

Timer::Timer()
{		 
	this->Next_Poll=0.;//per sec
	this->Collect_Poll=0;
	this->Average=0;
	this->dt=0.;//time
	this->Frame_Time=0.f;
}

void Timer::Start()
{
    // get ticks per second
    QueryPerformanceFrequency(&this->frequency);
    // start timer
    QueryPerformanceCounter(&this->Time_Hold);


	this->next_game_tick = GetTickCount();
}

void Timer::End()
{
	this->dt=0.0;//dont really need to end
}


const int FRAMES_PER_SECOND = 60;
const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

void Timer::Update()
{				 
	LARGE_INTEGER Temp_Time;
    QueryPerformanceCounter(&Temp_Time);	   

    // compute and print the elapsed time in millisec
	this->dt = (Temp_Time.QuadPart - this->Time_Hold.QuadPart) * 1000.f / this->frequency.QuadPart;

	this->Collect_Poll++;
	this->Next_Poll+=dt;
	if(this->Next_Poll>=1000.f)	// 1000dt == 1 second
	{
		this->Average=this->Collect_Poll;
		this->Collect_Poll=0;
		this->Next_Poll=0.;
	}

	this->next_game_tick += SKIP_TICKS;
    int sleep_time = this->next_game_tick - GetTickCount();
    if( sleep_time >= 0 ) {
        Sleep( sleep_time );
    }
	
	this->Time_Hold=Temp_Time;//update
}