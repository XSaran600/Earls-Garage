#pragma once
//Bade code by: Brent Cowan Jan. 22, 2018
//Code edited/altered by: Justin Collier | 100345263

//This pre-processor definition add the lib file in code instead of using the project settings.
//This makes adding your code to other projects easier, but it may not work on other operating
//systems or compilers.
#pragma comment (lib, "lib/fmod_vc.lib")

#include <Windows.h>
#include <math.h>
#include "inc/fmod.hpp"
#include "inc/fmod_errors.h"
#include <iostream> //cin & cout
#include <conio.h> //Keyboard input

using namespace std;

void FmodErrorCheck(FMOD_RESULT result);

class Listener
{
public:
	FMOD_VECTOR pos = { 0.0f, 0.0f, -1.0f };
	FMOD_VECTOR	forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR	up = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR	vel = { 0.0f, 0.0f, 0.0f };
};

class SoundEngine
{
private:
	bool init;
	FMOD_RESULT result;

public:
	SoundEngine();
	~SoundEngine();
	bool Init();
	void CleanUp();
	void update();

	FMOD::System    *system;
	unsigned int     version;
	void            *extradriverdata = 0;
	Listener         listener;
};