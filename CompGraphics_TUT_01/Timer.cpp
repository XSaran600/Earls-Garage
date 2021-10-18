#include "Timer.h"
#include "GL/glut.h"

Timer::Timer()
{
	previousTime = glutGet(GLUT_ELAPSED_TIME);
	tick();
}

Timer::~Timer()
{

}

//update the timer
float Timer::tick()
{
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	elapsedTime = currentTime - previousTime;
	previousTime = currentTime;
	return elapsedTime;
}

// return the delta time in milliseconds
float Timer::getElapsedTimeMS()
{
	return elapsedTime;
}

// return the delta time inn seconds
float Timer::getElapsedTimeSeconds()
{
	return elapsedTime / 1000.f;
}

// get time since program started
float Timer::getCurrentTime()
{
	return currentTime;
}
