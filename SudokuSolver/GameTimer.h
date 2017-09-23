// Jonathon Hamrick
//
// General purpose timer class
// 7/20/17

#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <Windows.h>

class GameTimer
{
public:
	GameTimer();

	void StartTime();
	DWORD ElapsedTime() const;
	//void Restart();
	DWORD TimeSlice();

private:
	DWORD TimerStartTime;
};

GameTimer::GameTimer()
{
	TimerStartTime = 0;
}

void GameTimer::StartTime()
{
	TimerStartTime = GetTickCount(); // start the clock
}

DWORD GameTimer::ElapsedTime() const
{
	return GetTickCount() - TimerStartTime; // return how long has elapsed since the last call to starttime or timeslice
}

DWORD GameTimer::TimeSlice()
{
	DWORD cT = GetTickCount(); // grab the current time
	DWORD dT = cT - TimerStartTime; // calculate change in time
	TimerStartTime = cT; // reset the start time
	return dT; // return change in time
}

#endif