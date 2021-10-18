#pragma once
#include "GameObject.h"

enum Bufftype
{
	BUFF_DEFAULT,
	BUFF_SPEED,
	BUFF_LEVEL,
	BUFF_REPAIR
};

class PowerUp
{
public:

	PowerUp();
	~PowerUp();

	//Functions
	void activateBuff(GameObject &MainCharacter, Bufftype type, float dt);
	void buffTimer(float dt);

	// Data members
	bool activeBuff = false;
	bool buffOn = false;
	bool powerUpExpire = false;
	int type = 0;

	float defaultSpeed = 0.2f;
	float speedBuff = 0.4f;
	float endTime = 0.f;
	int timerBuffDuration = 10;
	int repairBuffTime = -1;
	bool timeCheck = false;

	int temp = 0;

	// Data Members for timer
	bool onceTime = false; // Checks if the start timer has been started
	float startTime = 0.f; // Start time
	float buffEndTime = 0.f; // End time
	float expireTime = 10.f; //Time it takes for speed buff to expire
	float tempTime = 0.0f;
	bool timeDone = false;
	int tempTimeStore3 = 1;
	int tempTimeStore4 = 0;

	int currentBuff = BUFF_DEFAULT;
};