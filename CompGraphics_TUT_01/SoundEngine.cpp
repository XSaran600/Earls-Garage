//Bade code by: Brent Cowan Jan. 22, 2018
//Code edited/altered by: Justin Collier | 100345263
#include "SoundEngine.h"

void FmodErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		cout << "Fmod Error: " << FMOD_ErrorString(result) << endl;
		system("pause");
	}
}

SoundEngine::SoundEngine()
{
	//init = false;
}

SoundEngine::~SoundEngine()		//Deconstructor
{
	CleanUp();
}

void SoundEngine::CleanUp()		
{
	result = system->close();
	FmodErrorCheck(result);
	result = system->release();
	FmodErrorCheck(result);

	init = false;
}

void SoundEngine::update()		//Updates the sound.
{
	result = system->set3DListenerAttributes(0, &listener.pos, &listener.vel, &listener.forward, &listener.up);
	FmodErrorCheck(result);

	result = system->update();
	FmodErrorCheck(result);
}

bool SoundEngine::Init()			//Initialises the Sound Engine.
{
	if (!init)
	{
		init = true;

		result = FMOD::System_Create(&system);
		FmodErrorCheck(result);
		if (result != FMOD_OK) { init = false; }

		result = system->getVersion(&version);
		if (result != FMOD_OK) { init = false; }
		FmodErrorCheck(result);

		result = system->init(100, FMOD_INIT_NORMAL, extradriverdata);
		FmodErrorCheck(result);
		if (result != FMOD_OK) { init = false; }

		//Set the distance units. (meters/feet etc).
		result = system->set3DSettings(1.0, 1.0f, 1.0f);
		FmodErrorCheck(result);
		if (result != FMOD_OK) { init = false; return init; }
	}

	return init;
}