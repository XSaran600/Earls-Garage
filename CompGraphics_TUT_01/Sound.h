//Justin Collier | 100345263

#pragma once
#include "SoundEngine.h"

class Sound
{
public:
	Sound::Sound();
	Sound(SoundEngine *system);
	~Sound();

	FMOD::Sound *sound;
	//FMOD::Sound *effectSound;
	FMOD::Channel *channel = 0;
	//FMOD::Channel *channel2;
	FMOD_VECTOR position;
	FMOD_VECTOR velocity;

	bool load(const char* file, bool is3D, bool loop);
	bool updatePosition(FMOD::Channel*  channel, FMOD_VECTOR Pos, FMOD_VECTOR Vel);
	bool stopLoop();

	FMOD::Channel* Sound::play();

	FMOD_RESULT result;

	SoundEngine *sys;

};