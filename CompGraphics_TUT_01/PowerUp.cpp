#include "PowerUp.h"
#include <iostream>

PowerUp::PowerUp()
{

}

PowerUp::~PowerUp()
{

}

void PowerUp::activateBuff(GameObject &character, Bufftype type, float dt)
{
	currentBuff = type;

	if (currentBuff == BUFF_DEFAULT)
	{
		//Default (No Power-up)
	}

	//Increases Player Speed
	if (currentBuff == BUFF_SPEED)
	{	
		if (!powerUpExpire)
		{
			buffTimer(dt);
			if (character.playerSpeed != speedBuff)
			{
				character.playerSpeed = speedBuff;
			}

			if (timeDone)
			{
				std::cout << " Time done..." << std::endl;
				character.playerSpeed = defaultSpeed;
				std::cout << "Speed boost expired" << std::endl;
				character.energyDrink = false;
				powerUpExpire = true;
			}

		}
		
	}

	//Increases Level Time by 10.00 second(s).
	if (currentBuff == BUFF_LEVEL)
	{
		std::cout << "Level Time buff activated." << std::endl;
		character.levelTime += 10.f;
	}

	//Reduces repair times & part shipments by 1.00 Second(s).
	if (currentBuff == BUFF_REPAIR)
	{
		std::cout << "Repair buff activated." << std::endl;
		character.engineRepair = 2.5f;
		character.tireRepair = 2.f;
		character.paintRepair = 1.5f;
	}
}

void PowerUp::buffTimer(float dt)
{	
	if (onceTime == false)
	{
		startTime = dt;
		buffEndTime = startTime + expireTime;
		timeDone = false;
		onceTime = true;
	}

	tempTime = tempTime + dt;
	if (tempTime > buffEndTime)
	{
		onceTime = false;
		timeDone = true;
		tempTime = 0.0f;
	}
}; 