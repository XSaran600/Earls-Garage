#pragma once
#include <iostream>
#include <vector>

class AnimationID
{
public:
	AnimationID();
	~AnimationID();
	void setName(std::string name);
	void setNumFrames(unsigned int frames);
	void setStartFrame(unsigned int start);
	std::string getName();
	unsigned int getNumFrames();
	unsigned int getStartFrame();

	// Name of the Animation
	std::string AnimationName;

	// Number of Frames in the Animation
	unsigned int numFrames;

	// Where in the vector does the frame start at
	unsigned int startFrame;
};