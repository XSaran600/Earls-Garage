#include "AnimationID.h"

AnimationID::AnimationID()
{

}

AnimationID::~AnimationID()
{

}

void AnimationID::setName(std::string name)
{
	AnimationName = name;
}
void AnimationID::setNumFrames(unsigned int frames)
{
	numFrames = frames;
}
void AnimationID::setStartFrame(unsigned int start)
{
	startFrame = start;
}
std::string AnimationID::getName()
{
	return AnimationName;
}
unsigned int AnimationID::getNumFrames()
{
	return numFrames;
}
unsigned int AnimationID::getStartFrame()
{
	return startFrame;
}