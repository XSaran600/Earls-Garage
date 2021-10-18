#pragma once

//Keeps track of elaped time so we can use it for things like physics calculations
class Timer
{
public:
	Timer();
	~Timer();

	float tick(); //update timer
	float getElapsedTimeMS(); //ammount of time passed in milliseconds
	float getElapsedTimeSeconds(); //ammount of time passed in seconds
	float getCurrentTime(); //ammount of time passed since start of program



private:
	float currentTime, previousTime, elapsedTime;
};