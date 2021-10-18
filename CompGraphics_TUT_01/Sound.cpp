//Justin Collier | 100345263

#include "Sound.h"

Sound::Sound() {

}

Sound::Sound(SoundEngine *system)
{
	sys = system;
}
Sound::~Sound()
{

}

bool Sound::load(const char* file, bool is3D, bool loop)
{
	if (is3D)
	{
		result = sys->system->createSound(file, FMOD_3D, 0, &sound);
		FmodErrorCheck(result);
		result = sound->set3DMinMaxDistance(0.5f, 500.0f); // 0.5 metres to 50 metres of distance
		FmodErrorCheck(result);
	}
	else
	{
		result = sys->system->createSound(file, FMOD_DEFAULT, 0, &sound);
		FmodErrorCheck(result);
	}

	if (loop)
	{
		result = sound->setMode(FMOD_LOOP_NORMAL);
		FmodErrorCheck(result);
	}
	else
	{
		result = sound->setMode(FMOD_LOOP_OFF);
		FmodErrorCheck(result);
	}
	return true;
}

bool Sound::updatePosition(FMOD::Channel*  channel, FMOD_VECTOR Pos, FMOD_VECTOR Vel)
{
	
	result = channel->set3DAttributes(&Pos, &Vel);		//Updates the drum(sound) position as well as the velocity which is currently irrelevent due to its set values of 0.
	FmodErrorCheck(result);
	return true;
}

bool Sound::stopLoop()
{
	result = sound->setMode(FMOD_LOOP_OFF);
	FmodErrorCheck(result);

	return true;
}

FMOD::Channel* Sound::play()
{
	//Play sounds at certain positions
	result = sys->system->playSound(sound, 0, true, &channel);
	FmodErrorCheck(result);
	result = channel->set3DAttributes(&position, &velocity);
	FmodErrorCheck(result);
	result = channel->setPaused(false);
	FmodErrorCheck(result);
	return channel;
}