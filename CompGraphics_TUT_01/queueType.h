#pragma once
#include "GameObject.h"

class queueType
{
public:
	queueType();
	~queueType();

	// Adds new element to the rear of the queue
	void addQueue(GameObject* newElement);
	void deleteQueue();

	void initializeQueue();
	void destroyQueue();
	bool isEmptyQueue();
	bool isFullQueue();

	GameObject* front();
	GameObject* back();

	void setMaxQueueSize(int maxQ);

private:
	int maxQueueSize;
	int count;
	int queueFront;
	int queueRear;
	GameObject **list; // pointer to array that holds the queue elements

};