#pragma once

#ifndef NNC_EXPORT
#define NNC_EXPORT __declspec(dllexport)
#endif // !NNC_EXPORT

#include "Point3D.h"

//The number of doubles that need to be passed by a game to create a full VoiceSource
const unsigned int DOUBLES_PER_VOICE_SOURCE = DOUBLES_PER_POINT3D + 1;

class NNC_EXPORT VoiceSource : public Point3D
{
public:
	//constructors / destructor
	VoiceSource();
	VoiceSource(double x, double y, double z);
	virtual ~VoiceSource();

	//Member functions
	virtual double leftVolume(const Point3D& userPosition);
	virtual double rightVolume(const Point3D& userPosition);
	virtual short nextDistortion(double amplitude);

	//getters / setters
	double getVoiceLevel();
	void setVoiceLevel(double level);

protected:
	//the loudness of the voice source
	//used to calculate how far the voice will carry
	//and how loud it will sound
	double voiceLevel;
	//modifies the rate at which sound degrades over distance
	const double voiceFalloffModifier = 1;

};

