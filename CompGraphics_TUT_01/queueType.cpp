#include "queueType.h"
#include "assert.h"
#include <iostream>

queueType::queueType()
{
	//maxQueueSize = maxQ;
	//list = new GameObject[maxQ];
}

queueType::~queueType()
{

}

void queueType::initializeQueue()
{
	queueFront = 0;
	queueRear = maxQueueSize - 1;
	count = 0;
}

void queueType::addQueue(GameObject* newElement)
{
	if (!isFullQueue())
	{
		queueRear = (queueRear + 1) % maxQueueSize;

		count++;
		list[queueRear] = newElement;
	}
	else
	{
		std::cout << "Cannot add to a full queue" << std::endl;
	}
}

void queueType::deleteQueue()
{
	if (!isEmptyQueue())
	{
		list[queueFront] = NULL;
		count--;
		queueFront = (queueFront + 1) % maxQueueSize;
	}
	else
	{
		std::cout << "Cannot remove from an empty queue" << std::endl;
	}
}

bool queueType::isEmptyQueue()
{
	return(count == 0);
}

bool queueType::isFullQueue()
{
	return (count == maxQueueSize);
}

void queueType::destroyQueue()
{
	for (int i = 0; i < maxQueueSize; i++)
	{
		list[i] = NULL;
	}
	queueFront = 0;
	queueRear = maxQueueSize - 1;
	count = 0;
}

GameObject* queueType::front()
{
	assert(!isEmptyQueue());
	return list[queueFront];
}

GameObject* queueType::back()
{
	assert(!isEmptyQueue());
	return list[queueRear];
}

void queueType::setMaxQueueSize(int maxQ)
{
	maxQueueSize = maxQ;
	list = new GameObject*[maxQ];
}